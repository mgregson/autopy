#include "keypress.h"
#include "deadbeef_rand.h"
#include "keywords.h"
#include <ctype.h> /* For isupper() */

#if defined(IS_WINDOWS)
	#define usleep(x) Sleep((x) / 1000)
#else
	#include <unistd.h> /* For usleep() */
#endif

#if defined(IS_MACOSX)
	#include <ApplicationServices/ApplicationServices.h>
#elif defined(USE_X11)
	#include <X11/extensions/XTest.h>
	#include "xdisplay.h"
#endif

/* Convenience wrappers around ugly APIs. */
#if defined(IS_WINDOWS)
	#define WIN32_KEY_EVENT(key, flags) keybd_event(key, 0, flags, 0)
	#define WIN32_KEY_EVENT_WAIT(key, flags) \
		(WIN32_KEY_EVENT(key, flags), Sleep(DEADBEEF_RANDRANGE(0, 63)))
#elif defined(USE_X11)
	#define X_KEY_EVENT(display, key, is_press) \
		(XTestFakeKeyEvent(display, \
		                   XKeysymToKeycode(display, key), \
		                   is_press, CurrentTime), \
		 XFlush(display))
	#define X_KEY_EVENT_WAIT(display, key, is_press) \
		(X_KEY_EVENT(display, key, is_press), \
		 usleep(DEADBEEF_RANDRANGE(0, 62501)))
#endif

void toggleKeyCode(MMKeyCode code, const bool down, MMKeyFlags flags)
{
#if defined(IS_MACOSX)
	CGEventRef keyEvent = CGEventCreateKeyboardEvent(NULL,
	                                                 (CGKeyCode)code, down);
	assert(keyEvent != NULL);

	CGEventSetType(keyEvent, down ? kCGEventKeyDown : kCGEventKeyUp);
	CGEventSetFlags(keyEvent, flags);
	CGEventPost(kCGSessionEventTap, keyEvent);
	CFRelease(keyEvent);
#elif defined(IS_WINDOWS)
	const DWORD dwFlags = down ? 0 : KEYEVENTF_KEYUP;

	/* Parse modifier keys. */
	if (flags & MOD_META) WIN32_KEY_EVENT_WAIT(K_META, dwFlags);
	if (flags & MOD_ALT) WIN32_KEY_EVENT_WAIT(K_ALT, dwFlags);
	if (flags & MOD_CONTROL) WIN32_KEY_EVENT_WAIT(K_CONTROL, dwFlags);
	if (flags & MOD_SHIFT) WIN32_KEY_EVENT_WAIT(K_SHIFT, dwFlags);

	WIN32_KEY_EVENT(code, dwFlags);
#elif defined(USE_X11)
	Display *display = XGetMainDisplay();
	const Bool is_press = down ? True : False; /* Just to be safe. */

	/* Parse modifier keys. */
	if (flags & MOD_META) X_KEY_EVENT_WAIT(display, K_META, is_press);
	if (flags & MOD_ALT) X_KEY_EVENT_WAIT(display, K_ALT, is_press);
	if (flags & MOD_CONTROL) X_KEY_EVENT_WAIT(display, K_CONTROL, is_press);
	if (flags & MOD_SHIFT) X_KEY_EVENT_WAIT(display, K_SHIFT, is_press);

	X_KEY_EVENT(display, code, is_press);
#endif
}

INLINE void tapKeyCode(MMKeyCode code, MMKeyFlags flags)
{
	toggleKeyCode(code, true, flags);
	toggleKeyCode(code, false, flags);
}

INLINE void toggleKey(char c, const bool down, MMKeyFlags flags)
{
	if (isupper(c) && !(flags & MOD_SHIFT)) {
		flags |= MOD_SHIFT; /* Not sure if this is safe for all layouts. */
	}
	toggleKeyCode(keyCodeForChar(c), down, flags);
}

INLINE void tapKey(char c, MMKeyFlags flags)
{
	toggleKey(c, true, flags);
	toggleKey(c, false, flags);
}

#if defined(IS_MACOSX)
void toggleUniKey(char c, const bool down)
{
	/* This function relies on the convenient
	 * CGEventKeyboardSetUnicodeString(), which allows us to not have to
	 * convert characters to a keycode, but does not support adding modifier
	 * flags. It is therefore only used in typeString() and typeStringDelayed()
	 * -- if you need modifier keys, use the above functions instead. */
	UniChar ch = (UniChar)c; /* Convert to unsigned char */

	CGEventRef keyEvent = CGEventCreateKeyboardEvent(NULL, 0, down);
	if (keyEvent == NULL) {
		fputs("Could not create keyboard event.\n", stderr);
		return;
	}

	CGEventKeyboardSetUnicodeString(keyEvent, 1, &ch);

	CGEventPost(kCGSessionEventTap, keyEvent);
	CFRelease(keyEvent);
}
#else
	#define toggleUniKey(c, down) toggleKey(c, down, MOD_NONE)
#endif

static INLINE void tapUniKey(char c)
{
	toggleUniKey(c, true);
	toggleUniKey(c, false);
}

INLINE void typeString(const char *str)
{
	while (*str != '\0') {
		tapUniKey(*str++);
	}
}

void typeStringDelayed(const char *str, const unsigned cpm)
{
	/* Characters per second */
	const float cps = (float)cpm / 60.0f;

	/* Average micro-seconds per character */
	const unsigned uspc = cps == 0.0f ? 0 : (unsigned)(1000000.0f / cps);

	while (*str != '\0') {
		tapUniKey(*str++);
		usleep(uspc + (DEADBEEF_RANDRANGE(0, 62501)));
	}
}
