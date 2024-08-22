# Robot Contrôleur statique 
L'objectif de ce projet est de concevoir un système intelligent utilisant le potentiel complet du
kit "STEAMY Starter". Ce système agit comme un contrôleur statique, capable de surveiller 
et d'informer sur l'état d'un laboratoire en temps réel, en détectant divers défauts et en 
fournissant des alertes appropriées. Le projet met en œuvre plusieurs composants 
électroniques pour assurer une régulation fiable et automatique de l'environnement de travail.

# STEAMY STARTER
Steamy starter est un ensemble de carte Arduino Uno et d’un shield Arduino Uno sur lequel il 
y a plusieurs composants électroniques.
![steamy](Saved%20Pictures/steamy.jpg)

# PRE-REQUIS
  ## Matériels nécéssaires 
   - Steamy starter
   - telecommande infrarouge
   - Recepteur infrarouge
   - Capteur ultrason
   - Capteur d'humidité
   - Ecran LCD
  ## Librairies
   - LiquidCrystal_I2C
   - DHT
   - IRremote
# Montage
![steamy](Saved%20Pictures/steamy_montage.jpg)

# Fonctionnement du système
  ## 1. Démarrage du système

  - **Bouton poussoir BP1** : Le système est activé par un appui sur le bouton poussoir BP1. Une fois démarré, ce bouton sert également à acquitter les défauts après leur résolution.
  - **LED verte** : Lorsque le système est en marche et qu'aucun défaut n'est détecté, la LED verte s'allume pour indiquer que tout fonctionne correctement.
  - **Écran LCD** : Le message "PRÊT" est affiché à l'écran pour signaler que le système est prêt à fonctionner sans problème.
  
  ## 2. Détection des défauts
  
  - **LED rouge** : S’il y a un défaut (e.g. présence d'eau), la LED rouge s’allume.
  - **Capteur d'humidité** : Si de l'eau est détectée dans le laboratoire, le capteur déclenche une alerte. La LED rouge s'allume et le message "ATTENTION ELECTROCUTION" est affiché sur l'écran LCD. Ce défaut doit être acquitté en appuyant sur le bouton BP1 après avoir résolu le problème (élimination de l'eau).
  - **Capteur de lumière** : Le système détecte l'absence de lumière, allume la LED rouge et affiche le message "ATTENTION ACCIDENT" sur l'écran LCD, indiquant un risque de sécurité pour les opérateurs.
  
  ## 3. Gestion des obstacles
  
  - **Capteur ultrason** : Ce capteur mesure la distance à laquelle se trouve un obstacle. En fonction de la distance, la LED RGB change de couleur pour signaler la proximité du danger :
      - Entre 32 cm et 40 cm : La LED RGB est verte.
      - Entre 22 cm et 32 cm : La LED RGB est orange.
      - Moins de 22 cm : La LED RGB devient rouge, indiquant un risque immédiat.
  - **Écran LCD** : Lorsqu'un obstacle est détecté à moins de 40 cm, le message "OBSTACLE à X CM" est affiché à l'écran, avec X correspondant à la distance exacte mesurée.
  
  ## 4. Alertes en cas de défaut prolongé
  
  - **Buzzer** : Si un défaut n'est pas résolu ou acquitté dans un délai de 3 minutes, le buzzer retentit pour alerter l'opérateur que le système est resté trop longtemps en défaut. Cela peut se produire, par exemple, si de l'eau n'a pas été retirée après une détection.
  - **Télécommande infrarouge (IR)** : Si le buzzer a sonné pendant plus d'une minute sans que le défaut ne soit acquitté, la télécommande infrarouge peut être utilisée pour déclencher un jeu de lumières sur la LED RGB afin d'attirer l'attention des opérateurs.
  
  ## 5. Redémarrage et arrêt du système
  
  - **Bouton poussoir BP2** : Un appui simple sur le bouton poussoir BP2 permet de redémarrer le système après l'acquittement des défauts. Si l'on appuie longuement sur ce même bouton, le système s'éteint complètement.
  
# Resultat  
![steamy](Saved%20Pictures/pret.jpg)

Pour avoir plus d'informations sur Techitall [Visiter TIA](https://techitall.co)

