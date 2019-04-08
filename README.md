# Hackathon-W18
Hackathon submission for the 72-hour winter 2018.

A space-cowboy-in-a-matrix game for two players over the internet.
To run, you need to run the server (which unfortunately is hard-coded to be hack.hunterlannon.net -- change your etc hosts to point hack.hunterlannon.net to 127.0.0.1). When two people launch the client, they can shoot at each other, and if they are shot, they *rewind* back to their places that they stood moments ago. The game can play with 1 person, but there will be a dead-still cowboy  that doesn't do anything.

The game is rendered in 2D isometric fashion, and all the art and textures used herewithin are open to the public.

![Real Lyfe Scr33nSh0t](https://i.imgur.com/oi20VPo.png)

# Dependencies

* SFML-2.5 - https://www.sfml-dev.org/download/sfml/2.5.1/
* ENet - http://enet.bespin.org/Downloads.html

# Build

To build, just run `make`.

# Running

The build process generates two executables, `game` and `server`. You should run the server locally if you want multiplayer, but make sure your `/etc/hosts` file has the following entry:

```bash
127.0.0.1 hack.hunterlannon.net
```

The multiplayer should just **work**. Clients can come and go as they please, but only two players are allowed. I may add more players in the future :)

# Controls

wasd - move around the map.

\<space\> - tap to shoot. Hold to aim with wasd.
