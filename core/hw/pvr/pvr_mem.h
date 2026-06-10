#pragma once
#include "types.h"
#include "stdclass.h"
#include "hw/sh4/sh4_if.h"

//vram 32-64b
extern RamRegion vram;

//regs
u32 pvr_ReadReg(u32 addr);
void pvr_WriteReg(u32 paddr, u32 data);

void DYNACALL TAWrite(u32 address, const SQBuffer *data, u32 count);
void DYNACALL TAWriteSQ(u32 address, const SQBuffer *sqb);

void YUV_init();
void YUV_serialize(Serializer& ser);
void YUV_deserialize(Deserializer& deser);
void YUV_reset();

// 32-bit vram path handlers
inline u32 pvr_map32(u32 offset32)
{
	//64b wide bus is achieved by interleaving the banks every 32 bits
	constexpr u32 VRAM_BANK_BIT = 0x400000;
	const u32 static_bits = VRAM_MASK - (VRAM_BANK_BIT * 2 - 1) + 3;
	const u32 offset_bits = (VRAM_BANK_BIT - 1) & ~3;

	const u32 bank = (offset32 & VRAM_BANK_BIT) / VRAM_BANK_BIT;

	u32 rv = offset32 & static_bits;

	rv |= (offset32 & offset_bits) * 2;

	rv |= bank * 4;

	return rv;
}

template<typename T> T DYNACALL pvr_read32p(u32 addr);
template<typename T, bool Internal = false> void DYNACALL pvr_write32p(u32 addr, T data);
// Area 4 handlers
template<typename T, bool upper> T DYNACALL pvr_read_area4(u32 addr);
template<typename T, bool upper> void DYNACALL pvr_write_area4(u32 addr, T data);
