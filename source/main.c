#include "common.h"
#include "draw.h"
#include "fs.h"
#include "hid.h"
#include "menu.h"
#include "i2c.h"
#include "decryptor/game.h"
#include "decryptor/nand.h"
#include "decryptor/nandfat.h"
#include "decryptor/titlekey.h"

#define SUBMENU_START 5


MenuInfo menu[] =
{
    {
        "XORpad Generator Options", 4,
        {
            { "NCCH Padgen", &NcchPadgen, 0, 0, 0 },
            { "SD Padgen", &SdPadgen, 0, 0, 0 },
            { "CTRNAND Padgen", &CtrNandPadgen, 0, 0, 0 },
            { "TWLNAND Padgen", &TwlNandPadgen, 0, 0, 0 }
        }
    },
    {
        "Titlekey Decrypt Options", 3,
        {
            { "Titlekey Decrypt (file)", &DecryptTitlekeysFile, 0, 0, 0 },
            { "Titlekey Decrypt (NAND)", &DecryptTitlekeysNand, 0, 0, 0 },
            { "Titlekey Decrypt (EMU)", &DecryptTitlekeysNand, 0, 1, 0 }
        }
    },
    {
        "SysNAND Options", 8,
        {
            { "NAND Backup", &DumpNand, 0, 0, 0 },
            { "NAND Restore", &RestoreNand, 1, 0, 0 },
            { "Partition Dump...", NULL, 0, 0, SUBMENU_START + 0 },
            { "Partition Inject...", NULL, 1, 0, SUBMENU_START + 2 },
            { "File Dump...", NULL, 0, 0, SUBMENU_START + 4 },
            { "File Inject...", NULL, 1, 0, SUBMENU_START + 6 },
            { "Health&Safety Dump", &DumpHealthAndSafety, 0, 0, 0 },
            { "Health&Safety Inject", &InjectHealthAndSafety, 1, 0, 0 }
        }
    },
    {
        "EmuNAND Options", 9,
        {
            { "EmuNAND Backup", &DumpNand, 0, 0, 0 },
            { "EmuNAND Restore", &RestoreNand, 1, 0, 0 },
            { "Partition Dump...", NULL, 0, 0, SUBMENU_START + 1 },
            { "Partition Inject...", NULL, 1, 0, SUBMENU_START + 3 },
            { "File Dump...", NULL, 0, 0, SUBMENU_START + 5 },
            { "File Inject...", NULL, 1, 0, SUBMENU_START + 7 },
            { "Health&Safety Dump", &DumpHealthAndSafety, 0, 0, 0 },
            { "Health&Safety Inject", &InjectHealthAndSafety, 1, 0, 0 },
            { "Update SeedDB", &UpdateSeedDb, 0, 1, 0 }
        }
    },
    {
        "Game Decryptor Options", 6,
        {
            { "NCCH/NCSD Decryptor", &DecryptNcsdNcch, 0, 0, 0 },
            { "NCCH/NCSD Encryptor", &EncryptNcsdNcchStandard, 0, 0, 0 },
            { "CIA Decryptor (shallow)", &DecryptCiaShallow, 0, 0, 0 },
            { "CIA Decryptor (deep)", &DecryptCiaDeep, 0, 0, 0 },
            { "CIA Decryptor (for GW)", &DecryptCiaGateway, 0, 0, 0 },
            { "SD Decryptor/Encryptor", &CryptSdFiles, 0, 0, 0 }
        }
    },
    // everything below is not contained in the main menu
    {
        "Dump Partition (SysNAND)", 6, // ID 0
        {
            { "Dump TWLN Partition", &DecryptNandPartitions, 0, 0, P_TWLN },
            { "Dump TWLP Partition", &DecryptNandPartitions, 0, 0, P_TWLP },
            { "Dump AGBSAVE Partition", &DecryptNandPartitions, 0, 0, P_AGBSAVE },
            { "Dump FIRM0 Partition", &DecryptNandPartitions, 0, 0, P_FIRM0 },
            { "Dump FIRM1 Partition", &DecryptNandPartitions, 0, 0, P_FIRM1 },
            { "Dump CTRNAND Partition", &DecryptNandPartitions, 0, 0, P_CTRNAND }
        }
    },
    {
        "Dump Partition (EmuNAND)", 6, // ID 1
        {
            { "Dump TWLN Partition", &DecryptNandPartitions, 0, 1, P_TWLN },
            { "Dump TWLP Partition", &DecryptNandPartitions, 0, 1, P_TWLP },
            { "Dump AGBSAVE Partition", &DecryptNandPartitions, 0, 1, P_AGBSAVE },
            { "Dump FIRM0 Partition", &DecryptNandPartitions, 0, 1, P_FIRM0 },
            { "Dump FIRM1 Partition", &DecryptNandPartitions, 0, 1, P_FIRM1 },
            { "Dump CTRNAND Partition", &DecryptNandPartitions, 0, 1, P_CTRNAND }
        }
    },
    {
        "Inject Partition (SysNAND)", 6, // ID 2
        {
            { "Inject TWLN Partition", &InjectNandPartitions, 1, 0, P_TWLN },
            { "Inject TWLP Partition", &InjectNandPartitions, 1, 0, P_TWLP },
            { "Inject AGBSAVE Partition", &InjectNandPartitions, 1, 0, P_AGBSAVE },
            { "Inject FIRM0 Partition", &InjectNandPartitions, 1, 0, P_FIRM0 },
            { "Inject FIRM1 Partition", &InjectNandPartitions, 1, 0, P_FIRM1 },
            { "Inject CTRNAND Partition", &InjectNandPartitions, 1, 0, P_CTRNAND }
        }
    },
    {
        "Inject Partition (EmuNAND)", 6, // ID 3
        {
            { "Inject TWLN Partition", &InjectNandPartitions, 1, 1, P_TWLN },
            { "Inject TWLP Partition", &InjectNandPartitions, 1, 1, P_TWLP },
            { "Inject AGBSAVE Partition", &InjectNandPartitions, 1, 1, P_AGBSAVE },
            { "Inject FIRM0 Partition", &InjectNandPartitions, 1, 1, P_FIRM0 },
            { "Inject FIRM1 Partition", &InjectNandPartitions, 1, 1, P_FIRM1 },
            { "Inject CTRNAND Partition", &InjectNandPartitions, 1, 1, P_CTRNAND }
        }
    },
    {
        "Dump File (SysNAND)", 8, // ID 4
        {
            { "Dump ticket.db", &DumpFile, 0, 0, F_TICKET },
            { "Dump title.db", &DumpFile, 0, 0, F_TITLE },
            { "Dump import.db", &DumpFile, 0, 0, F_IMPORT },
            { "Dump certs.db", &DumpFile, 0, 0, F_CERTS },
            { "Dump SecureInfo_A", &DumpFile, 0, 0, F_SECUREINFO },
            { "Dump LocalFriendCodeSeed_B", &DumpFile, 0, 0, F_LOCALFRIEND },
            { "Dump rand_seed", &DumpFile, 0, 0, F_RANDSEED },
            { "Dump movable.sed", &DumpFile, 0, 0, F_MOVABLE }
        }
    },
    {
        "Dump File (EmuNAND)", 9, // ID 5
        {
            { "Dump ticket.db", &DumpFile, 0, 1, F_TICKET },
            { "Dump title.db", &DumpFile, 0, 1, F_TITLE },
            { "Dump import.db", &DumpFile, 0, 1, F_IMPORT },
            { "Dump certs.db", &DumpFile, 0, 1, F_CERTS },
            { "Dump SecureInfo_A", &DumpFile, 0, 1, F_SECUREINFO },
            { "Dump LocalFriendCodeSeed_B", &DumpFile, 0, 1, F_LOCALFRIEND },
            { "Dump rand_seed", &DumpFile, 0, 1, F_RANDSEED },
            { "Dump movable.sed", &DumpFile, 0, 1, F_MOVABLE },
            { "Dump seedsave.bin", &DumpFile, 0, 1, F_SEEDSAVE }
        }
    },
    {
        "Inject File (SysNAND)", 8, // ID 6
        {
            { "Inject ticket.db", &InjectFile, 1, 0, F_TICKET },
            { "Inject title.db", &InjectFile, 1, 0, F_TITLE },
            { "Inject import.db", &InjectFile, 1, 0, F_IMPORT },
            { "Inject certs.db", &InjectFile, 1, 0, F_CERTS },
            { "Inject SecureInfo_A", &InjectFile, 1, 0, F_SECUREINFO },
            { "Inject LocalFriendCodeSeed_B", &InjectFile, 1, 0, F_LOCALFRIEND },
            { "Inject rand_seed", &InjectFile, 1, 0, F_RANDSEED },
            { "Inject movable.sed", &InjectFile, 1, 0, F_MOVABLE }
        }
    },
    {
        "Inject File (EmuNAND)", 9, // ID 7
        {
            { "Inject ticket.db", &InjectFile, 1, 1, F_TICKET },
            { "Inject title.db", &InjectFile, 1, 1, F_TITLE },
            { "Inject import.db", &InjectFile, 1, 1, F_IMPORT },
            { "Inject certs.db", &InjectFile, 1, 1, F_CERTS },
            { "Inject SecureInfo_A", &InjectFile, 1, 1, F_SECUREINFO },
            { "Inject LocalFriendCodeSeed_B", &InjectFile, 1, 1, F_LOCALFRIEND },
            { "Inject rand_seed", &InjectFile, 1, 1, F_RANDSEED },
            { "Inject movable.sed", &InjectFile, 1, 1, F_MOVABLE },
            { "Inject seedsave.bin", &InjectFile, 1, 1, F_SEEDSAVE }
        }
    }
};


void Reboot()
{
    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 2);
    while(true);
}


void PowerOff()
{
    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 0);
    while (true);
}


int main()
{
    ClearScreenFull(false);
    ClearScreenFull(true);
    InitFS();

    u32 menu_exit = ProcessMenu(menu, SUBMENU_START);
    
    DeinitFS();
    (menu_exit == MENU_EXIT_REBOOT) ? Reboot() : PowerOff();
    return 0;
}
