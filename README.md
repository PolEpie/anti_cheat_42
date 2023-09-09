# 🚔 Anti Cheat 42

Hi! If like me your sometimes do you debuging your code using printf and you dont wanna be marked as cheater by the Moulinette this program is for you !

It will directly read all the .c files on the current directory and into all the subdirectories too and let you know if any of there a **printf** or even if you still have a **main** to avoid send your testing main

Also it will find every **a.out** files to delete them and also let you know if there is others files that `.c` or `.h` ones

Btw it avoid commented lines things so if you using `/* */` or `//` comments, anti cheat will not send you any message 😉

If you wanna another feature into it let me know on issues tab 😁

# Compilation

    gcc anti_cheat.c -o anti_cheat

## Usage

    ./anti_cheat

![enter image description here](https://cdn.discordapp.com/attachments/664897058867511328/1150196989343039600/CleanShot_2023-09-10_at_00.31.492x.png)
