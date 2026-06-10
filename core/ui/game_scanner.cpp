/*
	Copyright 2024 flyinghead

	This file is part of Flycast.

    Flycast is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Flycast is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Flycast.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "game_scanner.h"
#include "stdclass.h"
#include "oslib/oslib.h"
#include "oslib/storage.h"
#include "cfg/option.h"
#include "oslib/i18n.h"

static bool operator<(const GameMedia &left, const GameMedia &right)
{
	return (int)left.arcade < (int)right.arcade
			|| (left.arcade == right.arcade && i18n::locale()(left.name, right.name));
}

void GameScanner::add_game_directory(const std::string& path, std::vector<GameMedia>& discovered)
{
	hostfs::DirectoryTree tree(path);
	std::string emptyParentPath;
	for (const hostfs::FileInfo& item : tree)
	{
		if (!running)
			break;

		if (discovered.empty())
		{
			// This won't work for android content uris
			size_t slash = get_last_slash_pos(item.path);
			std::string parentPath;
			if (slash != 0 && slash != std::string::npos)
				parentPath = item.path.substr(0, slash);
			else
				parentPath = item.path;
			if (parentPath != emptyParentPath)
			{
				++empty_folders_scanned;
				emptyParentPath = parentPath;
				if (empty_folders_scanned > 1000)
					content_path_looks_incorrect = true;
			}
		}
		else
		{
			content_path_looks_incorrect = false;
		}

		if (item.name.substr(0, 2) == "._")
			// Ignore Mac OS turds
			continue;
		std::string fileName(item.name);
		std::string gameName(get_file_basename(item.name));
		std::string extension = get_file_extension(item.name);
		if (extension == "zip" || extension == "7z")
		{
			string_tolower(gameName);
			auto it = arcade_games.find(gameName);
			if (it == arcade_games.end())
				continue;
			gameName = it->second->description;
			fileName = fileName + " (" + gameName + ")";
			discovered.push_back(GameMedia{ fileName, item.path, item.name, gameName, true });
			continue;
		}
		else if (extension == "bin" || extension == "lst" || extension == "dat")
		{
			if (!config::HideLegacyNaomiRoms)
				discovered.push_back(GameMedia{ fileName, item.path, item.name, gameName, true });
			continue;
		}
		else if (extension == "chd" || extension == "gdi")
		{
			// Hide arcade gdroms
			std::string basename = gameName;
			string_tolower(basename);
			if (arcade_gdroms.count(basename) != 0)
				continue;
		}
		else if (extension != "cdi" && extension != "cue")
			continue;
		discovered.push_back(GameMedia{ fileName, item.path, item.name, gameName });
	}
}

void GameScanner::stop()
{
	LockGuard _(threadMutex);
	running = false;
    empty_folders_scanned = 0;
    content_path_looks_incorrect = false;
	if (scan_thread && scan_thread->joinable())
		scan_thread->join();
}

void GameScanner::fetch_game_list()
{
	LockGuard _(threadMutex);
	if (scan_done || running)
		return;
	if (scan_thread && scan_thread->joinable())
		scan_thread->join();
	running = true;
	scan_thread = std::make_unique<std::thread>([this]()
		{
			ThreadName _("GameScanner");
			if (arcade_games.empty())
				for (int gameid = 0; Games[gameid].name != nullptr; gameid++)
				{
					const Game *game = &Games[gameid];
					arcade_games[game->name] = game;
					if (game->gdrom_name != nullptr)
						arcade_gdroms.insert(game->gdrom_name);
				}
			std::vector<GameMedia> discovered;
			for (const auto& path : config::ContentPath.get())
			{
				try {
					add_game_directory(path, discovered);
				} catch (const hostfs::StorageException& e) {
					// ignore
				}
				if (!running)
					break;
			}
			std::sort(discovered.begin(), discovered.end());
			std::string dcbios = hostfs::findFlash("dc_", "%bios.bin;%boot.bin");
			{
				LockGuard _(mutex);
				game_list = std::move(discovered);
				if (!config::loadBool("config", "HideCdromDrives", false))
				{
					// CD-ROM devices
					for (const auto& drive : hostfs::getCdromDrives())
					{
						std::string name;
						if (drive.substr(0, 4) == "\\\\.\\")
							name = drive.substr(4);
						else
							name = drive;
						game_list.insert(game_list.begin(), { name, drive, name, "", false, true });
					}
				}
				// Dreamcast BIOS
				if (!dcbios.empty())
					game_list.insert(game_list.begin(), { i18n::T("Dreamcast BIOS") });
			}
			if (running)
				scan_done = true;
			running = false;
		});
}
