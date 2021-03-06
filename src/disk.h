#include "util.h"

#ifndef _FDISK_H_
#define _FDISK_H_

typedef struct {
    FILE *diskfile;
    char *fileName;

    size_t bytesPerSector;
    size_t totalSectors;
    size_t sectorsPerCluster;

    size_t reserved;

    size_t fatNum;
    size_t fatSize;
    size_t *fatPos;

    size_t rootEntries;
    size_t rootPos;
    size_t rootSize;

    size_t dataPos;
    size_t dataSize;

} disk_t;

static byte formatedDBR[512] = {0xeb,0x3c,0x90,0x4d,0x53,0x44,0x4f,0x53,0x35,0x2e,0x30,0x00,0x02,0x01,0x01,0x00,0x02,0xe0,0x00,0x40,0x0b,0xf0,0x09,0x00,0x12,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x29,0x0a,0x1f,0xe4,0x15,0x4e,0x4f,0x20,0x4e,0x41,0x4d,0x45,0x20,0x20,0x20,0x20,0x46,0x41,0x54,0x31,0x36,0x20,0x20,0x20,0xfa,0x33,0xc0,0x8e,0xd0,0xbc,0x00,0x7c,0x16,0x07,0xbb,0x78,0x00,0x36,0xc5,0x37,0x1e,0x56,0x16,0x53,0xbf,0x3e,0x7c,0xb9,0x0b,0x00,0xfc,0xf3,0xa4,0x06,0x1f,0xc6,0x45,0xfe,0x0f,0x8b,0x0e,0x18,0x7c,0x88,0x4d,0xf9,0x89,0x47,0x02,0xc7,0x07,0x3e,0x7c,0xfb,0xcd,0x13,0x72,0x79,0x33,0xc0,0x39,0x06,0x13,0x7c,0x74,0x08,0x8b,0x0e,0x13,0x7c,0x89,0x0e,0x20,0x7c,0xa0,0x10,0x7c,0xf7,0x26,0x16,0x7c,0x03,0x06,0x1c,0x7c,0x13,0x16,0x1e,0x7c,0x03,0x06,0x0e,0x7c,0x83,0xd2,0x00,0xa3,0x50,0x7c,0x89,0x16,0x52,0x7c,0xa3,0x49,0x7c,0x89,0x16,0x4b,0x7c,0xb8,0x20,0x00,0xf7,0x26,0x11,0x7c,0x8b,0x1e,0x0b,0x7c,0x03,0xc3,0x48,0xf7,0xf3,0x01,0x06,0x49,0x7c,0x83,0x16,0x4b,0x7c,0x00,0xbb,0x00,0x05,0x8b,0x16,0x52,0x7c,0xa1,0x50,0x7c,0xe8,0x92,0x00,0x72,0x1d,0xb0,0x01,0xe8,0xac,0x00,0x72,0x16,0x8b,0xfb,0xb9,0x0b,0x00,0xbe,0xe6,0x7d,0xf3,0xa6,0x75,0x0a,0x8d,0x7f,0x20,0xb9,0x0b,0x00,0xf3,0xa6,0x74,0x18,0xbe,0x9e,0x7d,0xe8,0x5f,0x00,0x33,0xc0,0xcd,0x16,0x5e,0x1f,0x8f,0x04,0x8f,0x44,0x02,0xcd,0x19,0x58,0x58,0x58,0xeb,0xe8,0x8b,0x47,0x1a,0x48,0x48,0x8a,0x1e,0x0d,0x7c,0x32,0xff,0xf7,0xe3,0x03,0x06,0x49,0x7c,0x13,0x16,0x4b,0x7c,0xbb,0x00,0x07,0xb9,0x03,0x00,0x50,0x52,0x51,0xe8,0x3a,0x00,0x72,0xd8,0xb0,0x01,0xe8,0x54,0x00,0x59,0x5a,0x58,0x72,0xbb,0x05,0x01,0x00,0x83,0xd2,0x00,0x03,0x1e,0x0b,0x7c,0xe2,0xe2,0x8a,0x2e,0x15,0x7c,0x8a,0x16,0x24,0x7c,0x8b,0x1e,0x49,0x7c,0xa1,0x4b,0x7c,0xea,0x00,0x00,0x70,0x00,0xac,0x0a,0xc0,0x74,0x29,0xb4,0x0e,0xbb,0x07,0x00,0xcd,0x10,0xeb,0xf2,0x3b,0x16,0x18,0x7c,0x73,0x19,0xf7,0x36,0x18,0x7c,0xfe,0xc2,0x88,0x16,0x4f,0x7c,0x33,0xd2,0xf7,0x36,0x1a,0x7c,0x88,0x16,0x25,0x7c,0xa3,0x4d,0x7c,0xf8,0xc3,0xf9,0xc3,0xb4,0x02,0x8b,0x16,0x4d,0x7c,0xb1,0x06,0xd2,0xe6,0x0a,0x36,0x4f,0x7c,0x8b,0xca,0x86,0xe9,0x8a,0x16,0x24,0x7c,0x8a,0x36,0x25,0x7c,0xcd,0x13,0xc3,0x0d,0x0a,0x4e,0x6f,0x6e,0x2d,0x53,0x79,0x73,0x74,0x65,0x6d,0x20,0x64,0x69,0x73,0x6b,0x20,0x6f,0x72,0x20,0x64,0x69,0x73,0x6b,0x20,0x65,0x72,0x72,0x6f,0x72,0x0d,0x0a,0x52,0x65,0x70,0x6c,0x61,0x63,0x65,0x20,0x61,0x6e,0x64,0x20,0x70,0x72,0x65,0x73,0x73,0x20,0x61,0x6e,0x79,0x20,0x6b,0x65,0x79,0x20,0x77,0x68,0x65,0x6e,0x20,0x72,0x65,0x61,0x64,0x79,0x0d,0x0a,0x00,0x49,0x4f,0x20,0x20,0x20,0x20,0x20,0x20,0x53,0x59,0x53,0x4d,0x53,0x44,0x4f,0x53,0x20,0x20,0x20,0x53,0x59,0x53,0x00,0x00,0x55,0xaa};

// 返回一个填充好默认信息的disk_t
// 用于完全全新的磁盘文件
disk_t * createDisk(char *);
// 用于一个正确的已存在的磁盘文件
disk_t * checkDisk(char *);
void destroyDisk(disk_t *);

disk_t * genInfo(disk_t *);
disk_t * chkInfo(disk_t *);
static disk_t * calcInfo(disk_t *);

disk_t * fdisk(disk_t *);

#endif
