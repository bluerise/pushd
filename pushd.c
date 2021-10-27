/* $OpenBSD$ */
/*
 * Copyright (c) 2019 Patrick Wildt <patrick@blueri.se>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#include "curl/curl.h"

/* #define PUSHD_TOKEN "YOUR APITOKEN" */
/* #define PUSHD_USER "YOUR USERTOKEN" */
/* #define PUSHD_PROXY "http://your.proxy.example.org:8181" */
/* #define PUSHD_IGNORE "/bsd: ignore this" */

void push(char *);

int
main(int argc, char *argv[])
{
	char chunk[1024];
	char *line;
	size_t len;
#ifdef PUSHD_IGNORE
	size_t i;
#endif

	curl_global_init(CURL_GLOBAL_ALL);

	for (;;) {
		line = NULL;
		getline(&line, &len, stdin);
		if (line == NULL)
			break;
#ifdef PUSHD_IGNORE
		for (i = 0; i < len; i++) {
			if (strlen(PUSHD_IGNORE) >= len - i)
				break;
			if (strncmp(line + i, PUSHD_IGNORE,
			    strlen(PUSHD_IGNORE)) == 0) {
				free(line);
				line = NULL;
				break;
			}
		}
#endif
		strlcpy(chunk, line, sizeof(chunk));
		push(chunk);
		free(line);
	}

	curl_global_cleanup();
	return 0;
}

void
push(char *msg)
{
	CURL *curl;
	char *opts = NULL;
	char *output;

	if (msg == NULL || strlen(msg) == 0)
		return;

	curl = curl_easy_init();
	if (curl == NULL)
		return;
	output = curl_easy_escape(curl, msg, 0);
	asprintf(&opts, "token=" PUSHD_TOKEN "&user=" PUSHD_USER
	    "&message=%s", output);
	curl_free(output);
	
	if (opts == NULL)
		goto out;

	curl_easy_setopt(curl, CURLOPT_URL, "https://api.pushover.net/1/messages.json");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, opts);
#ifdef PUSHD_PROXY
	curl_easy_setopt(curl, CURLOPT_PROXY, PUSHD_PROXY);
#endif

	curl_easy_perform(curl);
out:
	curl_easy_cleanup(curl);
}
