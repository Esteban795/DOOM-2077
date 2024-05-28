= PPII2 - CR6

== Présents:

- Tom BLOCH
- Timothée BRILLAUD (fait semblant d'être absent)
- Axelle HUMBERT
- Zoé LE GLEUT
- Esteban MANDJUKOWSKI (chef suprême du parti)

== Ordre du jour

- Mise au point
- Définir le sprint des 2 prochaines semaines

== Mise au point

=== Tom 

- Avancée sur les armes, animation quand tu tires pas ou que tu marches, début d'animation de tir,

=== Zoé

- Cooldows et sprays des armes,

=== Timothée

n'est pas là. A priori, il a bossé sur les HP et le scoreboard

=== Esteban

- Portes finito 
- Ascenseurs finito
- Render : ça s'adapte à la hauteur du joueur et de là où est le sprite, ça clip horizontalement mais pas verticalement

=== Axelle

- UI : finito 
- Menus : un début. 

== Sprint 19->21

Prochaine réunion avant dernière semaine (le mercredi inshallah)

=== Tom

- Animations de tirs 
- Refactor pour que ce soit mieux 
=== Zoé 

- fonction pour déterminer quelle sprite afficher selon les deux angles des joueurs 
- texturer la map custom

=== Timothée

- Implémenter portes et ascenseurs dans l'ECS et les broadcast à chaque tick 

=== Esteban

- Clip vertical des sprites
- Fonction pour savoir si une touche est maintenue 
- réfléchir avec Timothée comment il gère ses updates de tick 
- fix le bug des linedefs 
- voir pour afficher le chat 
- mettre des commentaires dans le code
- rajouter le son des portes, des ascenseurs et des tirs


=== Axelle

- UI:
  - Chat
  - Settings
  - Menu
  - Scoreboard (Kills,Deaths)
- HUD
- Crosshair
- texturer la map
- volume en fonction de la distance