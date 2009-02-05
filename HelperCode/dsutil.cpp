/*
 * Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
 
#include "dsutil.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *
copyString(char *s)
{
	int len;
	char *t;

	if (s == NULL) return NULL;

	len = strlen(s) + 1;
	t = (char*)malloc(len);
	memmove(t, s, len);
	return t;
}

char *
concatString(char *s, char *t)
{
	int len;

	if (t == NULL) return s;

	len = strlen(s) + strlen(t) + 1;
	s = (char*)realloc(s, len);
	strcat(s, t);
	return s;
}

char **
insertString(char *s, char **l, unsigned int x)
{
	unsigned int i, len;

	if (s == NULL) return l;
	if (l == NULL) 
	{
		l = (char **)malloc(2 * sizeof(char *));
		l[0] = copyString(s);
		l[1] = NULL;
		return l;
	}

	for (i = 0; l[i] != NULL; i++);
	len = i + 1; /* count the NULL on the end of the list too! */

	l = (char **)realloc(l, (len + 1) * sizeof(char *));

	if ((x >= (len - 1)) || (x == IndexNull))
	{
		l[len - 1] = copyString(s);
		l[len] = NULL;
		return l;
	}

	for (i = len; i > x; i--) l[i] = l[i - 1];
	l[x] = copyString(s);
	return l;
}

char **
appendString(char *s, char **l)
{
	return insertString(s, l, IndexNull);
}

void
freeList(char **l)
{
	int i;

	if (l == NULL) return;
	for (i = 0; l[i] != NULL; i++)
	{
		if (l[i] != NULL) free(l[i]);
		l[i] = NULL;
	}
	free(l);
}

void
freeString(char *s)
{
	if (s == NULL) return;
	free(s);
}

unsigned int
listLength(char **l)
{
	int i;

	if (l == NULL) return 0;
	for (i = 0; l[i] != NULL; i++);
	return i;
}

unsigned int
listIndex(char *s, char **l)
{
	int i;

	if (l == NULL) return IndexNull;
	for (i = 0; l[i] != NULL; i++)
	{
		if (strcmp(s, l[i]) == 0) return i;
	}
	return IndexNull;
}

char *
prefix(char *s, char c)
{
	int i;
	char *t;

	if (s == NULL) return NULL;

	for (i = 0; ((s[i] != '\0') && (s[i] != c)); i++);
	if (i == 0) return NULL;
	if (s[i] == '\0') return copyString(s);

	t = (char*)malloc(i + 1);
	bcopy(s, t, i);
	t[i] = '\0';
	return t;
}

char *
postfix(char *s, char c)
{
	int i, len;
	char *t;

	if (s == NULL) return NULL;

	for (i = 0; ((s[i] != '\0') && (s[i] != c)); i++);
	if (s[i] == '\0') return NULL;
	len = strlen(s) - i;
	if (len == 1) return NULL;

	t = (char*)malloc(len);
	len--;
	bcopy((s + i + 1), t, len);
	t[len] = '\0';
	return t;
}
	
char *
presuffix(char *s, char c)
{
	int i, len;
	char *t;

	if (s == NULL) return NULL;

	len = strlen(s);
	for (i = len - 1; ((i >= 0) && (s[i] != c)); i--);
	if (i == 0) return NULL;
	if (s[0] == '\0') return NULL;

	t = (char*)malloc(i + 1);
	bcopy(s, t, i);
	t[i] = '\0';
	return t;
}

char *
suffix(char *s, char c)
{
	int i, len;
	char *t;

	if (s == NULL) return NULL;

	len = strlen(s);
	for (i = len - 1; ((i >= 0) && (s[i] != c)); i--);
	if (i == 0) return NULL;
	len -= i;
	if (len == 1) return NULL;
	t = (char*)malloc(len);
	len--;
	bcopy((s + i + 1), t, len);
	t[len] = '\0';
	return t;
}

char *
lowerCase(char *s)
{
	int i;
	char *t;

	if (s == NULL) return NULL;
	t = (char*)malloc(strlen(s) + 1);

	for (i = 0; s[i] != '\0'; i++) 
	{
		if ((s[i] >= 'A') && (s[i] <= 'Z')) t[i] = s[i] + 32;
		else t[i] = s[i];
	}
	t[i] = '\0';
	return t;
}

unsigned int
stringIndex(char c, char *s)
{
	int i;
	char *p;

	if (s == NULL) return IndexNull;

	for (i = 0, p = s; p[0] != '\0'; p++, i++)
	{
		if (p[0] == c) return i;
	}

	return IndexNull;
}

char **
explode(char *s, char *delim)
{
	char **l = NULL;
	char *p, *t;
	int i, n;

	if (s == NULL) return NULL;

	p = s;
	while (p[0] != '\0')
	{
		for (i = 0; ((p[i] != '\0') && (stringIndex(p[i], delim) == IndexNull)); i++);
		n = i;
		t = (char*)malloc(n + 1);
		for (i = 0; i < n; i++) t[i] = p[i];
		t[n] = '\0';
		l = appendString(t, l);
		free(t);
		t = NULL;
		if (p[i] == '\0') return l;
		if (p[i + 1] == '\0') l = appendString("", l);
		p = p + i + 1;
	}
	return l;
}

char *
itoa(int n)
{
	char s[64];

	sprintf(s, "%d", n);
	return copyString(s);
}

