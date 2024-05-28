#ifndef _LIB_DOOM_EVENT_EVENT_H
#define _LIB_DOOM_EVENT_EVENT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/** Event representation.
 *
 * The tag is a unique identifier for the event type.  
 * The info is a variable-length array of bytes that contains the event data.  
 * The length of the info array is determined by the tag.  
 * 
 * To be useful, an event must be casted to its specialized type.
 */
typedef struct {
    /* The tag is a unique identifier for the event type.
     *
     * The most significant bit is used to distinguish between server and client events, as most events 
     * will be similar but not identical between the server and client.
     */
    uint16_t tag;
    /// The info is a variable-length array of bytes that contains the event data.
    uint8_t info[];
} event_t;

/// Helper function to check if an event is a server event.
bool is_server_event(event_t* e);

/// Helper function to check if an event is a client event.
bool is_client_event(event_t* e);

#endif