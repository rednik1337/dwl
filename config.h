/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }
/* appearance */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const int smartgaps                 = 0;  /* 1 means no outer gap when there is only one window */
static int gaps                            = 1;  /* 1 means gaps between windows are added */
static const unsigned int gappx            = 8; /* gap pixel between windows */
static const unsigned int borderpx         = 2;  /* border pixel of windows */
static const float rootcolor[]             = COLOR(0x161616ff);
static const float bordercolor[]           = COLOR(0x2a372abb);
static const float focuscolor[]            = COLOR(0x4e6533cc);
static const float urgentcolor[]           = COLOR(0xa6a69cff);
/* This conforms to the xdg-protocol. Set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.1f, 0.1f, 0.1f, 1.0f}; /* You can also use glsl colors */

/* tagging - TAGCOUNT must be no greater than 31 */
#define TAGCOUNT (9)

/* logging */
static int log_level = WLR_ERROR;

/* Autostart */
static const char *const autostart[] = {
        "foot", "--server", 0,
        "swaybg", "-m", "fill", "-i", "/home/aeterna/pictures/wall.jpg", 0,
        "dunst", 0,
        0
};


/* NOTE: ALWAYS keep a rule declared even if you don't use rules (e.g leave at least one example) */
static const Rule rules[] = {
	/* app_id             title       tags mask     isfloating   monitor */
	/* examples: */
	{ "Gimp_EXAMPLE",     NULL,       0,            1,           -1 }, /* Start on currently visible tags floating, not tiled */
	{ "firefox_EXAMPLE",  NULL,       1 << 8,       0,           -1 }, /* Start on ONLY tag "9" */
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* monitors */
/* (x=-1, y=-1) is reserved as an "autoconfigure" monitor position indicator
 * WARNING: negative values other than (-1, -1) cause problems with Xwayland clients
 * https://gitlab.freedesktop.org/xorg/xserver/-/issues/899
*/
/* NOTE: ALWAYS add a fallback rule, even if you are completely sure it won't be used */
static const MonitorRule monrules[] = {
	/* name       mfact nmaster scale layout       rotate/reflect              x  y  resx resy rate mode adaptive*/
	/* example of a HiDPI laptop monitor at 120Hz:
	{ "eDP-1",    0.5f,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL, 0, 0, 0, 0, 120.000f, 1, 1},
	* mode let's the user decide on how dwl should implement the modes:
	* -1 Sets a custom mode following the users choice
	* All other number's set the mode at the index n, 0 is the standard mode; see wlr-randr
	*/
    { "DP-1",     0.55f, 1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL, 0, 0, 3440, 1440, 144.000f, 1,   0 },
    { 0,          0.55f, 1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL, -1,-1,-1,   -1,   -1,       -1,  -1 },
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* can specify fields: rules, model, layout, variant, options */
	/* example:
	.options = "ctrl:nocaps",
	*/
        .options = "grp:alt_space_toggle",
        .layout = "us,ru",
};

static const int repeat_rate = 25;
static const int repeat_delay = 600;

/* Trackpad */
static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 0;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
static const double accel_speed = -1.0;

/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

static const int cursor_timeout = 3;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_LOGO

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, KEY,            tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,KEY,toggletag,  {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[] = { "footclient", NULL };
static const char *menucmd[] = { "wmenu-run", NULL };

#include "keys.h"
static const Key keys[] = {
	/* modifier                  key          function        argument */
	{ MODKEY,                    Key_space,       spawn,          {.v = menucmd} },
	{ MODKEY,                    Key_q,  spawn,          {.v = termcmd} },
	{ MODKEY,                    Key_j,       focusstack,     {.i = +1} },
	{ MODKEY,                    Key_k,       focusstack,     {.i = -1} },
	{ MODKEY,                    Key_i,       incnmaster,     {.i = +1} },
	{ MODKEY,                    Key_d,       incnmaster,     {.i = -1} },
	{ MODKEY,                    Key_h,       setmfact,       {.f = -0.05f} },
	{ MODKEY,                    Key_l,       setmfact,       {.f = +0.05f} },
	//{ MODKEY,                    Key_Return,  zoom,           {0} },
	//{ MODKEY,                    Key_Tab,     view,           {0} },
	{ MODKEY,                    Key_c,       killclient,     {0} },
	//{ MODKEY,                    Key_t,       setlayout,      {.v = &layouts[0]} },
	//{ MODKEY,                    Key_f,       setlayout,      {.v = &layouts[1]} },
	//{ MODKEY,                    Key_m,       setlayout,      {.v = &layouts[2]} },
	//{ MODKEY,                    Key_space,   setlayout,      {0} },
	{ MODKEY,                    Key_v,   togglefloating, {0} },
	{ MODKEY,                    Key_f,       togglefullscreen, {0} },
	//{ MODKEY,                    Key_0,       view,           {.ui = ~0} },
	//{ MODKEY|WLR_MODIFIER_SHIFT, Key_0,       tag,            {.ui = ~0} },
	//{ MODKEY,                    Key_comma,   focusmon,       {.i = WLR_DIRECTION_LEFT} },
	//{ MODKEY,                    Key_period,  focusmon,       {.i = WLR_DIRECTION_RIGHT} },
	//{ MODKEY|WLR_MODIFIER_SHIFT, Key_comma,   tagmon,         {.i = WLR_DIRECTION_LEFT} },
	//{ MODKEY|WLR_MODIFIER_SHIFT, Key_period,  tagmon,         {.i = WLR_DIRECTION_RIGHT} },
        {MODKEY,                     Key_s,         spawn,          SHCMD("grim -g \"$(slurp)\" - | wl-copy")},

	TAGKEYS(                     Key_1,                       0),
	TAGKEYS(                     Key_2,                       1),
	TAGKEYS(                     Key_3,                       2),
	TAGKEYS(                     Key_4,                       3),
	TAGKEYS(                     Key_5,                       4),
	TAGKEYS(                     Key_6,                       5),
	TAGKEYS(                     Key_7,                       6),
	TAGKEYS(                     Key_8,                       7),
	TAGKEYS(                     Key_9,                       8),
	{ MODKEY|WLR_MODIFIER_SHIFT, Key_Delete,       quit,           {0} },

	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,Key_BackSpace, quit, {0} },
#define CHVT(KEY,n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT, KEY, chvt, {.ui = (n)} }
	/* Ctrl-Alt-Fx is used to switch to another VT, if you don't know what a VT is
	 * do not remove them.
	 */
	CHVT(Key_F1, 1), CHVT(Key_F2,  2),  CHVT(Key_F3,  3),  CHVT(Key_F4,  4),
	CHVT(Key_F5, 5), CHVT(Key_F6,  6),  CHVT(Key_F7,  7),  CHVT(Key_F8,  8),
	CHVT(Key_F9, 9), CHVT(Key_F10, 10), CHVT(Key_F11, 11), CHVT(Key_F12, 12),
};

static const Button buttons[] = {
	{ MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ MODKEY, BTN_MIDDLE, togglefloating, {0} },
	{ MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
};
