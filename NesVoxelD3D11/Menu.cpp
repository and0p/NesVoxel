#include "pch.h"
#include "Menu.h"

#define NES_SUBMENU_NO 1
#define CONFIG_SUBMENU_NO 2
#define REGISTER_OFFSET 3	// Gotta include the divider!!!

#define OVERRIDE_OPTION_COUNT 6

struct RegisterBinding {
	RegisterOption option;
	UINT groupID;
	UINT menuID[OVERRIDE_OPTION_COUNT];
};

RegisterBinding registerMenuItems[REGISTER_OPTION_SIZE] = {
	mirror_selection, 0, { ID_MIRROR_NOO, ID_MIRROR_VERTICAL, ID_MIRROR_HORIZONTAL, ID_MIRROR_SINGLE, ID_MIRROR_FULL, ID_MIRROR_DIAGONAL },
	oam_pattern, 1 ,{ ID_OAMTABLE_NOO, ID_OAMTABLE_LEFT, ID_OAMTABLE_RIGHT, 0, 0, 0 },
	nt_pattern, 2, { ID_NTTABLE_NOO, ID_NTTABLE_LEFT, ID_NTTABLE_RIGHT, 0, 0, 0 },
	sprite8x16, 3, { ID_SPRITE8X16_NOO, ID_SPRITE8X16_DISABLED, ID_SPRITE8X16_ENABLED, 0, 0, 0 },
	render_oam, 4, { ID_RENDEROAM_NOO, ID_RENDEROAM_DISABLED, ID_RENDEROAM_ENABLED, 0, 0, 0 },
	render_nt, 5, { ID_RENDERNT_NOO, ID_RENDERNT_DISABLED, ID_RENDERNT_ENABLED, 0, 0, 0 },
	render_oam_left, 6, { ID_RENDERLEFTOAM_NOO, ID_RENDERLEFTOAM_DISABLED, ID_RENDERLEFTOAM_ENABLED, 0, 0, 0 },
	render_nt_left, 7, { ID_RENDERLEFTNT_NOO, ID_RENDERLEFTNT_DISABLED, ID_RENDERLEFTNT_ENABLED, 0, 0, 0 },
	greyscale, 8, { ID_GREYSCALE_NOO, ID_GREYSCALE_DISABLED, ID_GREYSCALE_ENABLED, 0, 0, 0 },
	emphasize_red, 9, { ID_EMPHASIZERED_NOO, ID_EMPHASIZERED_DISABLED, ID_EMPHASIZERED_ENABLED, 0, 0, 0 },
	emphasize_green, 10, { ID_EMPHASIZEGREEN_NOO, ID_EMPHASIZEGREEN_DISABLED, ID_EMPHASIZEGREEN_ENABLED, 0, 0, 0 },
	emphasize_blue, 11, { ID_EMPHASIZEBLUE_NOO, ID_EMPHASIZEBLUE_DISABLED, ID_EMPHASIZEBLUE_ENABLED, 0, 0, 0 }
};

void updateMenu(HMENU hmenu, bool gameLoaded)
{
	// Get the submenus
	HMENU nesSubmenu = GetSubMenu(hmenu, NES_SUBMENU_NO);
	// Enable and disable relevant controls if game has not yet loaded
	if (gameLoaded)
	{
		EnableMenuItem(nesSubmenu, ID_NES_POWER, MF_ENABLED);
		EnableMenuItem(nesSubmenu, ID_NES_RESET, MF_ENABLED);
	}
	else
	{
		EnableMenuItem(nesSubmenu, ID_NES_POWER, MF_DISABLED);
		EnableMenuItem(nesSubmenu, ID_NES_RESET, MF_DISABLED);
	}
	// Update register display
	for (UINT i = 0; i < REGISTER_OPTION_SIZE; i++)
	{
		RegisterOption o = (RegisterOption)i;
		// See if register active, overridden or not, and check the menu item if so
		if (N3sConfig::isRegisterActive(o))
		{
			CheckMenuItem(nesSubmenu, REGISTER_OFFSET + registerMenuItems[i].groupID, MF_CHECKED | MF_BYPOSITION);
		}
		else
		{
			CheckMenuItem(nesSubmenu, REGISTER_OFFSET + registerMenuItems[i].groupID, MF_UNCHECKED | MF_BYPOSITION);
		}
		// Check the correct submenu item, indicating override status
		HMENU registerSubmenu = GetSubMenu(nesSubmenu, REGISTER_OFFSET + registerMenuItems[i].groupID);
		for (int r = 0; r < OVERRIDE_OPTION_COUNT; r++)
			CheckMenuItem(registerSubmenu, r, ((N3sConfig::registers[o] == r) ? MF_CHECKED : MF_UNCHECKED) | MF_BYPOSITION);
		// Set group label bold (or asterisk?) to indicate override status
	}
}