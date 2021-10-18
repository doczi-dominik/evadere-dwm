/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrainsMono Nerd Font Mono:size=10:weight=Bold" };
static const char dmenufont[]       = "monospace:size=10";
static const char background[]      = "#000000";
static const char foreground[]      = "#FFFFFF";
static const char sub[]             = "#646464";
static const char accent[]          = "#FFC637";
static const unsigned int baralpha = 0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { background, background, sub },
	[SchemeSel]  = { background, background, accent },
	[SchemeStatus]  = { foreground, background, background }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = { accent, background,  background }, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm]  = { foreground, background, background }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]  = { background, background, background }, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm]  = { background,background, background }, // infobar middle  unselected {text,background,not used but cannot be empty}
};
static const unsigned int alphas[][3] = {
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel] = { OPAQUE, baralpha, borderalpha },
	[SchemeStatus] = { OPAQUE, baralpha, borderalpha },
	[SchemeTagsSel] = { OPAQUE, baralpha, borderalpha },
	[SchemeTagsNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeInfoSel] = { OPAQUE, baralpha, borderalpha },
	[SchemeInfoNorm] = { OPAQUE, baralpha, borderalpha },
};

static const char *const autostart[] = {
	"picom", "-CGb", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/* include(s) depending on the tags array */
#include "flextile.h"

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{  NULL,      NULL,       NULL,       0,            0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int layoutaxis[] = {
	1,    /* layout axis: 1 = x, 2 = y; negative values mirror the layout, setting the master area to the right / bottom instead of left / top */
	2,    /* master axis: 1 = x (from left to right), 2 = y (from top to bottom), 3 = z (monocle) */
	2,    /* stack axis:  1 = x (from left to right), 2 = y (from top to bottom), 3 = z (monocle) */
};
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "rofi", "-modi", "drun", "-show", "drun", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *togglemutecmd[] = { "pamixer", "--toggle-mute", NULL };
static const char *raisevolcmd[] = { "pamixer", "--increase", "2", NULL };
static const char *lowervolcmd[] = { "pamixer", "--decrease", "2", NULL };
static const char *brightencmd[] = { "light", "-A", "2", NULL };
static const char *darkencmd[] = { "light", "-U", "2", NULL };

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,                     spawn,            {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,                spawn,            {.v = termcmd } },
	{ NULL,                         XF86XK_AudioMute,         spawn,            {.v = togglemutecmd } },
	{ NULL,                         XF86XK_AudioRaiseVolume,  spawn,            {.v = raisevolcmd } },
	{ NULL,                         XF86XK_AudioLowerVolume,  spawn,            {.v = lowervolcmd } },
	{ NULL,                         XF86XK_MonBrightnessUp,   spawn,            {.v = brightencmd } },
	{ NULL,                         XF86XK_MonBrightnessDown, spawn,            {.v = darkencmd } },
	{ MODKEY,                       XK_j,                     focusstack,       {.i = +1 } },
	{ MODKEY,                       XK_k,                     focusstack,       {.i = -1 } },
	{ MODKEY,                       XK_i,                     shiftmastersplit, {.i = +1} },   /* increase the number of tiled clients in the master area */
	{ MODKEY,                       XK_u,                     shiftmastersplit, {.i = -1} },   /* reduce the number of tiled clients in the master area */
	{ MODKEY,                       XK_h,                     setmfact,         {.f = -0.05} },
	{ MODKEY,                       XK_l,                     setmfact,         {.f = +0.05} },
	{ MODKEY,                       XK_Tab,                   zoom,             {0} },
	{ MODKEY,                       XK_q,                     killclient,       {0} },
	{ MODKEY,		                XK_comma,                 cyclelayout,      {.i = -1 } },
	{ MODKEY,                       XK_period,                cyclelayout,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_space,                 togglefloating,   {0} },
	{ MODKEY|ShiftMask,             XK_f,                     togglefullscr,    {0} },
	{ MODKEY,                       XK_0,                     view,             {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                     tag,              {.ui = ~0 } },
	{ MODKEY,                       XK_udiaeresis,            focusmon,         {.i = -1 } },
	{ MODKEY,                       XK_oacute,                focusmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_udiaeresis,            tagmon,           {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_oacute,                tagmon,           {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask, XK_q,                     quit,             {1} },
	{ MODKEY,                       XK_t,                     rotatelayoutaxis, {.i = 0} },    /* 0 = layout axis */
	{ MODKEY|ShiftMask,             XK_t,                     rotatelayoutaxis, {.i = 1} },    /* 1 = master axis */
	{ MODKEY|ControlMask,           XK_t,                     rotatelayoutaxis, {.i = 2} },    /* 2 = stack axis */
	{ MODKEY|ShiftMask|ControlMask, XK_t,                     mirrorlayout,     {0} },
	TAGKEYS(                        XK_1,                                       0)
	TAGKEYS(                        XK_2,                                       1)
	TAGKEYS(                        XK_3,                                       2)
	TAGKEYS(                        XK_4,                                       3)
	TAGKEYS(                        XK_5,                                       4)
	TAGKEYS(                        XK_6,                                       5)
	TAGKEYS(                        XK_7,                                       6)
	TAGKEYS(                        XK_8,                                       7)
	TAGKEYS(                        XK_9,                                       8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

