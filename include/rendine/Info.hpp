#pragma once

/* Straight from GNU manual */
#define __str_m(s) #s
#define __xstr_m(s) __str_m(s)

#define RENDINE_VERSION_MAJOR 0
#define RENDINE_VERSION_MINOR 1
#define RENDINE_VERSION_PATCH 1
#define RENDINE_VERSION_STAGE "a"

#define RENDINE_VERSION_STRING_V(major, minor, patch, stage) \
	__xstr_m(major) "." __xstr_m(minor) "." __xstr_m(patch) stage
#define RENDINE_VERSION_STRING() \
	RENDINE_VERSION_STRING_V(RENDINE_VERSION_MAJOR, RENDINE_VERSION_MINOR, RENDINE_VERSION_PATCH, RENDINE_VERSION_STAGE)
