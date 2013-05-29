/*
Copyright (c) by respective owners including Yahoo!, Microsoft, and
individual contributors. All rights reserved.  Released under a BSD
license as described in the file LICENSE.
 */

#ifndef HASH_H
#define HASH_H

const uint32_t hash_base = 0;
uint32_t uniform_hash( const void *key, size_t length, uint32_t initval);

#endif
