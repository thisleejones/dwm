/* See LICENSE file for copyright and license details. */

/* appearance */
static const char selbgcolor[]      = "#005577";
static const char selfgcolor[]      = "#eeeeee";
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 6;        /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=14" };
static const char dmenufont[]       = "monospace:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const unsigned int baralpha = 0xb0;
static const unsigned int borderalpha = 0xd0;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_gray1,  col_cyan  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/** Function to shift the current view to the left/right
 *
 * @param: "arg->i" stores the number of tags to shift right (positive value)
 *          or left (negative value)
 */
void
shiftview(const Arg *arg) {
	Arg shifted;

	if(arg->i > 0) // left circular shift
		shifted.ui = (selmon->tagset[selmon->seltags] << arg->i)
		   | (selmon->tagset[selmon->seltags] >> (LENGTH(tags) - arg->i));

	else // right circular shift
		shifted.ui = selmon->tagset[selmon->seltags] >> (- arg->i)
		   | selmon->tagset[selmon->seltags] << (LENGTH(tags) + arg->i);

	view(&shifted);
}

/* tagging */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	/* class      instance    title       tags mask    iscentered     isfloating   isterminal noswallow monitor */
	{ "Gimp",     NULL,       NULL,       0,           0,             1,           0,         0,        -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,      0,             0,           0,         0,        -1 },
	{ "st",       NULL,       NULL,       0,           0,             0,           1,         1,        -1 },
};

/* layout(s) */
static const float mfact     = 0.333333; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "layouts.c"
#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "|M|",	centeredmaster },    /* first entry is default */
	{ "[]=",	tile },
	{ "[M]",	monocle },
 	{ "[@]",	spiral },
	{ "><>",	NULL },              /* no layout function means floating behavior */
	{ ">M>",	centeredfloatingmaster },
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
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *browsercmd[] = { "qutebrowser", NULL };
static const char *editorcmd[] = { "emacs", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,		                XK_w,		killclient,	{0} },
	{ MODKEY|ControlMask|ShiftMask,	XK_w,		quit,	    {0} },
	{ MODKEY,			            XK_b,		togglebar,	{0} },
	{ MODKEY,			            XK_q,		spawn,	    {.v = browsercmd } },
	{ MODKEY,			            XK_e,		spawn,	    {.v = editorcmd } },
	{ MODKEY,                       XK_j,		focusstack,	{.i = +1 } },
	{ MODKEY,                       XK_k,		focusstack,	{.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,		movestack,	{.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,		movestack,	{.i = -1 } },
	{ MODKEY,			            XK_space,	zoom,		{0} },
	{ MODKEY|ShiftMask,             XK_h,		setmfact,	{.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_l,		setmfact,	{.f = +0.05} },
	{ MODKEY|ShiftMask,		        XK_i,		incnmaster,	{.i = +1 } },
	{ MODKEY|ShiftMask,		        XK_o,		incnmaster,	{.i = -1 } },
	{ MODKEY,			            XK_t,		setlayout,	{.v = &layouts[1]} },
	{ MODKEY,			            XK_f,		setlayout,	{.v = &layouts[4]} },
	{ MODKEY,			            XK_m,		setlayout,	{.v = &layouts[2]} },
	{ MODKEY,			            XK_u,		setlayout,	{.v = &layouts[0]} },
	{ MODKEY,			            XK_y,		setlayout,	{.v = &layouts[3]} },
	{ MODKEY,			            XK_o,		setlayout,	{.v = &layouts[5]} },
	{ MODKEY,			            XK_Tab,		view,		{0} },
	{ MODKEY,			            XK_backslash,	view,	{0} },
	{ MODKEY|ShiftMask,		        XK_space,	togglefloating,	{0} },
	/* { MODKEY,                       XK_space,  setlayout,      {0} }, */
	/* { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } }, */
	/* { MODKEY,                       XK_period, focusmon,       {.i = +1 } }, */
	/* { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } }, */
	/* { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } }, */
	TAGKEYS(			XK_1,			0)
	TAGKEYS(			XK_2,			1)
	TAGKEYS(			XK_3,			2)
	TAGKEYS(			XK_4,			3)
	TAGKEYS(			XK_5,			4)
	TAGKEYS(			XK_6,			5)
	TAGKEYS(			XK_7,			6)
	TAGKEYS(			XK_8,			7)
	TAGKEYS(			XK_9,			8)
	{ MODKEY,                       XK_0,		view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,		XK_0,		tag,		{.ui = ~0 } },
	{ MODKEY,			XK_F2,		quit,		{0} },
	{ MODKEY,			XK_g,		shiftview,	{ .i = -1 } },
	{ MODKEY,			XK_semicolon,	shiftview,	{ .i = 1 } },
	{ MODKEY,			XK_Page_Up,	shiftview,	{ .i = -1 } },
	{ MODKEY,			XK_Page_Down,	shiftview,	{ .i = 1 } },
	{ MODKEY,           XK_s,      swapfocus,      {.i = -1 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	/* { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } }, */
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

void
setlayoutex(const Arg *arg)
{
	setlayout(&((Arg) { .v = &layouts[arg->i] }));
}

void
viewex(const Arg *arg)
{
	view(&((Arg) { .ui = 1 << arg->ui }));
}

void
viewall(const Arg *arg)
{
	view(&((Arg){.ui = ~0}));
}

void
toggleviewex(const Arg *arg)
{
	toggleview(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagex(const Arg *arg)
{
	tag(&((Arg) { .ui = 1 << arg->ui }));
}

void
toggletagex(const Arg *arg)
{
	toggletag(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagall(const Arg *arg)
{
	tag(&((Arg){.ui = ~0}));
}

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signame> [<type> <value>]"` */
static Signal signals[] = {
	/* signum           function */
	{ "focusstack",     focusstack },
	{ "setmfact",       setmfact },
	{ "togglebar",      togglebar },
	{ "incnmaster",     incnmaster },
	{ "togglefloating", togglefloating },
	{ "focusmon",       focusmon },
	{ "tagmon",         tagmon },
	{ "zoom",           zoom },
	{ "view",           view },
	{ "viewall",        viewall },
	{ "viewex",         viewex },
	{ "toggleview",     view },
	{ "toggleviewex",   toggleviewex },
	{ "tag",            tag },
	{ "tagall",         tagall },
	{ "tagex",          tagex },
	{ "toggletag",      tag },
	{ "toggletagex",    toggletagex },
	{ "killclient",     killclient },
	{ "quit",           quit },
	{ "setlayout",      setlayout },
	{ "setlayoutex",    setlayoutex },
};
