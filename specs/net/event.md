# Events

| Date of creation | Latest edit      | Version | Status (adopted?) |
|------------------|------------------|---------|-------------------|
| 27/04/2024       | 27/04/2024       | alpha   | Experimental      |

## Global events

* [x] 0x0001 - PlayerJoinEvent {CS} - a player has joined the party
* [x] 0x0002 - PlayerQuitEvent {CS} - a player has quitted the party
* [ ] 0x0003 - GameStartCooldownEvent {CS} - start the cooldown before the party actually starts
* [ ] 0x0004 - GameStartEvent {CS} - start the party
* [ ] 0x0005 - GameEndEvent {CS} - end the party

## Player-specific events

* [x] 0x0040 - PlayerMoveEvent {CS} - a player is moving to a position
* [x] 0x0041 - PlayerDamageEvent {CS} - a player takes damages
* [x] 0x0042 - PlayerHealEvent {CS} - a player has been healed
* [x] 0x0043 - PlayerHealthUpdateEvent {CS} - a player's health has been changed.
* [x] 0x0044 - PlayerKilledEvent {CS} - a player has been killed
* [ ] 0x0045 - PlayerTriggerEvent {S} - a player triggers a button/door
* [ ] 0x0046 - PlayerFireEvent {S} - a player fires a weapon

## Door/Elevator events

* [ ] 0x0080 - DoorOpenEvent {CS} - open a door
* [ ] 0x0081 - DoorCloseEvent {CS} - close a door
* [ ] 0x0082 - ElevatorTriggeredEvent {CS} - trigger an elevator

## Communication events

* [x] 0x00c0 - PlayerChatEvent {CS} - a player sent a message in the chat
* [x] 0x00c1 - ServerChatEvent {CS} - the server sent a messsage in the chat
* [x] 0x00c2 - ScoreboardUpdateEvent {C} - the scoreboard should be updated.