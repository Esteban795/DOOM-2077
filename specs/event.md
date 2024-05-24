# Event System

| Date of creation | Latest edit      | Version | Status (adopted?) |
|------------------|------------------|---------|-------------------|
| 27/04/2024       | 17/05/2024       | beta   | Experimental      |

## Global events

* [x] 0x0001 - PlayerJoinEvent {CS} - a player has joined the party
* [x] 0x0002 - PlayerQuitEvent {CS} - a player has quitted the party
* [ ] 0x0003 - GameStartsInEvent {S} - the game will begin in XXs (0 -> game launches)
* [ ] 0x0004 - GameEndsInEvent {S} - the game will ends in XXs (0 -> game stops)

## Player-specific events

* [x] 0x0040 - PlayerMoveEvent {CS} - a player is moving to a position
* [x] 0x0041 - PlayerDamageEvent {CS} - a player takes damages
* [x] 0x0042 - PlayerHealEvent {CS} - a player has been healed
* [x] 0x0043 - PlayerHealthUpdateEvent {CS} - a player's health has been changed.
* [x] 0x0044 - PlayerKilledEvent {CS} - a player has been killed
* [ ] 0x0045 - PlayerFireEvent {S} - a player fires a weapon

## Door/Elevator events

* 0x0080 - DoorStateEvent {S} - set the current state of the doors and lifts
* [x] 0x0081 - DoorOpenEvent {CS} - a player is opening a door (lift ascending)
* [x] 0x0082 - DoorCloseEvent {CS} - a player is closing a door (lift descending) 

## Communication events

* [x] 0x00c0 - PlayerChatEvent {CS} - a player sent a message in the chat
* [x] 0x00c1 - ServerChatEvent {CS} - the server sent a messsage in the chat
* [x] 0x00c2 - ScoreboardUpdateEvent {C} - the scoreboard should be updated.