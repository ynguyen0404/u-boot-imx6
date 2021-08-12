/*
 * Copyright 2017 NXP
 * Copyright 2018-2019 Variscite Ltd.
 * 
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __MX8M_VAR_DART_H
#define __MX8M_VAR_DART_H

#include <linux/sizes.h>
#include <asm/arch/imx-regs.h>

#ifdef CONFIG_SECURE_BOOT
#define CONFIG_CSF_SIZE			0x2000 /* 8K region */
#endif

#define CONFIG_SPL_TEXT_BASE		0x7E1000
#define CONFIG_SPL_MAX_SIZE		(148 * 1024)
#define CONFIG_SYS_MONITOR_LEN		(512 * 1024)
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_USE_SECTOR
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	0x300
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION	1

#ifdef CONFIG_SPL_BUILD
#define CONFIG_SPL_WATCHDOG_SUPPORT
#define CONFIG_SPL_DRIVERS_MISC_SUPPORT
#define CONFIG_SPL_POWER_SUPPORT
#define CONFIG_SPL_I2C_SUPPORT
#define CONFIG_SPL_LDSCRIPT		"arch/arm/cpu/armv8/u-boot-spl.lds"
#define CONFIG_SPL_STACK		0x187FF0
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_GPIO_SUPPORT
#define CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SPL_BSS_START_ADDR      0x00180000
#define CONFIG_SPL_BSS_MAX_SIZE        0x2000	/* 8 KB */
#define CONFIG_SYS_SPL_MALLOC_START    0x42200000
#define CONFIG_SYS_SPL_MALLOC_SIZE     0x80000	/* 512 KB */
#define CONFIG_SYS_SPL_PTE_RAM_BASE    0x41580000
#define CONFIG_SYS_ICACHE_OFF
#define CONFIG_SYS_DCACHE_OFF

#define CONFIG_MALLOC_F_ADDR		0x182000 /* malloc f used before GD_FLG_FULL_MALLOC_INIT set */

#define CONFIG_SPL_ABORT_ON_RAW_IMAGE /* For RAW image gives a error info not panic */

#undef CONFIG_DM_MMC

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE100
#define CONFIG_POWER_PFUZE100_I2C_ADDR 0x08
#endif

#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#define CONFIG_SYS_I2C_MXC_I2C3		/* enable I2C bus 3 */

#define CONFIG_REMAKE_ELF

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_POSTCLK_INIT
#define CONFIG_BOARD_LATE_INIT

/* Flat Device Tree Definitions */
#define CONFIG_OF_BOARD_SETUP

#undef CONFIG_CMD_EXPORTENV
#undef CONFIG_CMD_IMPORTENV
#undef CONFIG_CMD_IMLS

#undef CONFIG_CMD_CRC32
#undef CONFIG_BOOTM_NETBSD

/* ENET Config */
/* ENET1 */
#if defined(CONFIG_CMD_NET)
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#define CONFIG_MII
#define CONFIG_ETHPRIME                 "FEC"

#define CONFIG_FEC_MXC
#define CONFIG_FEC_XCV_TYPE             RGMII
#define CONFIG_FEC_MXC_PHYADDR          0
#define FEC_QUIRK_ENET_MAC

#define CONFIG_PHY_GIGE
#define IMX_FEC_BASE			0x30BE0000

#define CONFIG_PHYLIB
#define CONFIG_PHY_ATHEROS
#endif

/*
 * Another approach is add the clocks for inmates into clks_init_on
 * in clk-imx8mq.c, then clk_ingore_unused could be removed.
 */
#define JAILHOUSE_ENV \
	"jh_clk= \0 " \
	"jh_mmcboot=setenv fdt_file imx8m-var-dart-root.dtb; " \
		"setenv jh_clk clk_ignore_unused; " \
			   "if run loadimage; then " \
				   "run mmcboot; " \
			   "else run jh_netboot; fi; \0" \
	"jh_netboot=setenv fdt_file imx8m-var-dart-root.dtb; setenv jh_clk clk_ignore_unused; run netboot; \0 "

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	JAILHOUSE_ENV \
	"bootdir=/boot\0"	\
	"script=boot.scr\0" \
	"image=Image.gz\0" \
	"console=ttymxc0,115200 earlycon=ec_imx6q,0x30860000,115200\0" \
	"img_addr=0x42000000\0"			\
	"fdt_addr=0x43000000\0"			\
	"fdt_high=0xffffffffffffffff\0"		\
	"boot_fdt=try\0" \
	"ip_dyn=yes\0" \
	"fdt_file=undefined\0" \
	"initrd_addr=0x43800000\0"		\
	"initrd_high=0xffffffffffffffff\0" \
	"video=HDMI-A-1:1920x1080-32@60\0" \
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" \
	"mmcblk=1\0" \
	"mmcautodetect=yes\0" \
	"mmcpart=1\0" \
	"m4_addr=0x7e0000\0" \
	"m4_bin=hello_world.bin\0" \
	"use_m4=no\0" \
	"loadm4bin=load mmc ${mmcdev}:${mmcpart} ${m4_addr} ${bootdir}/${m4_bin}\0" \
	"runm4bin=" \
		"if test ${m4_addr} = 0x7e0000; then " \
			"echo Booting M4 from TCM; " \
		"else " \
			"echo Booting M4 from DRAM; " \
			"dcache flush; " \
		"fi; " \
		"bootaux ${m4_addr};\0" \
	"optargs=setenv bootargs ${bootargs} ${kernelargs};\0" \
	"mmcargs=setenv bootargs console=${console} " \
		"root=/dev/mmcblk${mmcblk}p${mmcpart} rootfstype=ext4 rootwait rw " \
		"video=${video} ${cma_size}\0" \
	"loadbootscript=load mmc ${mmcdev}:${mmcpart} ${loadaddr} ${bootdir}/${script};\0" \
	"bootscript=echo Running bootscript from mmc ...; " \
		"source\0" \
	"loadimage=load mmc ${mmcdev}:${mmcpart} ${img_addr} ${bootdir}/${image};" \
		"unzip ${img_addr} ${loadaddr}\0" \
	"ramsize_check="\
		"if test $sdram_size -le 512; then " \
			"setenv cma_size cma=320M; " \
		"else " \
			"if test $sdram_size -le 1024; then " \
				"setenv cma_size cma=640M; " \
			"else " \
				"if test ${use_m4} = yes; then " \
					"setenv cma_size cma=928M@1088M; " \
				"else " \
					"setenv cma_size cma=960M@1088M; " \
				"fi; " \
			"fi; " \
		"fi;\0" \
	"findfdt=" \
		"if test $fdt_file = undefined; then " \
			"if gpio input 12; then " \
				"setenv fdt_file fsl-imx8mq-var-dart-cb12.dtb; " \
			"else " \
				"setenv fdt_file fsl-imx8mq-var-dart.dtb;" \
			"fi; " \
		"fi; \0" \
	"loadfdt=run findfdt; " \
		"echo fdt_file=${fdt_file}; " \
		"load mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${bootdir}/${fdt_file}\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run mmcargs; " \
		"run optargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"booti ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"echo WARN: Cannot load the DT; " \
			"fi; " \
		"else " \
			"echo wait for boot; " \
		"fi;\0" \
	"netargs=setenv bootargs console=${console} " \
		"root=/dev/nfs video=${video} ${cma_size} " \
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
	"netboot=echo Booting from net ...; " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${img_addr} ${image}; unzip ${img_addr} ${loadaddr};" \
		"run ramsize_check; " \
		"run netargs; " \
		"run optargs; " \
		"run findfdt; " \
		"echo fdt_file=${fdt_file}; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if ${get_cmd} ${fdt_addr} ${fdt_file}; then " \
				"booti ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"echo WARN: Cannot load the DT; " \
			"fi; " \
		"else " \
			"booti; " \
		"fi;\0" \
	"splashsourceauto=yes\0" \
	"splashfile=/boot/splash.bmp\0" \
	"splashimage=0x43100000\0" \
	"splashenable=setenv splashfile /boot/splash.bmp; " \
		"setenv splashimage 0x43100000\0" \
	"splashdisable=setenv splashfile; setenv splashimage\0"

#define CONFIG_BOOTCOMMAND \
	   "run ramsize_check; " \
	   "mmc dev ${mmcdev}; if mmc rescan; then " \
		   "if test ${use_m4} = yes && run loadm4bin; then " \
			   "run runm4bin; " \
		   "fi; " \
		   "if run loadbootscript; then " \
			   "run bootscript; " \
		   "else " \
			   "if run loadimage; then " \
				   "run mmcboot; " \
			   "else run netboot; " \
			   "fi; " \
		   "fi; " \
	   "else booti ${loadaddr} - ${fdt_addr}; fi"

/* Link Definitions */
#define CONFIG_LOADADDR			0x40480000
#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

#define CONFIG_SYS_INIT_RAM_ADDR        0x40000000
#define CONFIG_SYS_INIT_RAM_SIZE        0x80000
#define CONFIG_SYS_INIT_SP_OFFSET \
        (CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
        (CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#define CONFIG_ENV_OVERWRITE
#define CONFIG_ENV_OFFSET               (64 * SZ_64K)
#define CONFIG_ENV_SIZE			0x1000
#define CONFIG_SYS_MMC_ENV_DEV		1   /* USDHC2 */

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		((CONFIG_ENV_SIZE + (2*1024) + (16*1024)) * 1024)

#define CONFIG_SYS_SDRAM_BASE           0x40000000
#define PHYS_SDRAM                      0x40000000
#define DEFAULT_DRAM_SIZE_MB            512
#define CONFIG_NR_DRAM_BANKS            1

#define CONFIG_SYS_MEMTEST_START	PHYS_SDRAM
#define CONFIG_SYS_MEMTEST_END 		(CONFIG_SYS_MEMTEST_START + (256 * SZ_1M))

#define CONFIG_BAUDRATE			115200

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART1_BASE_ADDR

/* Monitor Command Prompt */
#undef CONFIG_SYS_PROMPT
#define CONFIG_SYS_PROMPT		"u-boot=> "
#define CONFIG_SYS_PROMPT_HUSH_PS2     "> "
#define CONFIG_SYS_CBSIZE              2048
#define CONFIG_SYS_MAXARGS             64
#define CONFIG_SYS_BARGSIZE CONFIG_SYS_CBSIZE
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_IMX_BOOTAUX

#define CONFIG_CMD_MMC
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC

#define CONFIG_SYS_FSL_USDHC_NUM	2
#define CONFIG_SYS_FSL_ESDHC_ADDR       0

#define CONFIG_SUPPORT_EMMC_BOOT	/* eMMC specific */

#define CONFIG_MXC_GPIO

#define CONFIG_MXC_OCOTP
#define CONFIG_CMD_FUSE

/* I2C Configs */
#define CONFIG_SYS_I2C_SPEED		  100000

/* USB configs */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_CMD_USB
#define CONFIG_USB_STORAGE
#define CONFIG_CMD_USB_MASS_STORAGE
#define CONFIG_USB_GADGET_MASS_STORAGE
#define CONFIG_USB_FUNCTION_MASS_STORAGE
#define CONFIG_CMD_READ
#endif

#define CONFIG_SERIAL_TAG
#define CONFIG_FASTBOOT_USB_DEV 0

#define CONFIG_USB_MAX_CONTROLLER_COUNT         2

#define CONFIG_USBD_HS
#define CONFIG_USB_GADGET_VBUS_DRAW 2

#define CONFIG_OF_SYSTEM_SETUP

/* Framebuffer */
#ifdef CONFIG_VIDEO
#define CONFIG_VIDEO_IMXDCSS
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_SPLASH_SCREEN
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_LOGO
#define CONFIG_VIDEO_BMP_LOGO
#define CONFIG_IMX_VIDEO_SKIP
#endif

/* Splash Screen  */
#ifdef CONFIG_SPLASH_SCREEN
#define CONFIG_CMD_BMP
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_SPLASH_SOURCE
#endif

#if defined(CONFIG_ANDROID_SUPPORT)
#include "imx8m_var_dart_android.h"
#elif defined (CONFIG_ANDROID_THINGS_SUPPORT)
#include "imx8m_var_dart_androidthings.h"
#endif
#endif
