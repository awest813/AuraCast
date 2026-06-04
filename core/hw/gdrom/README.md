# GD-ROM: Gigabyte Disc Read-Only Memory

- This is the GD-ROM emulation part v3. v1 was unusable and v2 was our initial
  release.

### Notes

- **Technical approach is wrong** — the v3 state machine in `gdromv3.cpp` is
  acknowledged as structurally incorrect; a redesign is tracked in
  `docs/ROADMAP.md` Phase 1.2, not a FIXME-only pass.
- **Secondary features incomplete** — SPI `0x70` mount/auth
  sequencing (`gdrom_hle.cpp`), multi-track CUE FAD math, and imgread sector
  conversion remain open.

### Audit status (2026-06-04)

| Item | Status |
|------|--------|
| `GDCC_GETTOC` HLE stub | Fixed — delegates to `GDROM_HLE_ReadTOC()` |
| CD read sector settings (`SPI_CD_READ`) | Improved — `gdrom_cd_read_sector_type()` covers common head/subh/data/other/expdtype combinations |
| PIO buffer length check | Fixed — `verify(len <= 0xFFFF)` |
| DMA vs PIO routing | Already handled via `Features.CDRead.DMA`; sector type now shared |
| SPI `0x70` status/interrupt | **Open** — needs hardware validation |
| HLE multi-read DMA callback | **Fixed** — `invoke_multi_callback()` on transfer end and `G1_DMA_END`, either order |
| HLE `GET_CMD_STAT` NBA 2K workaround | **Open** — `GDC_CONTINUE` + zero count promoted to `GDC_COMPLETE` |
| imgread CUE mixed sector sizes / CDI seek | **Open** |
| v3 architectural redesign | **Deferred** — strategic refactor |
