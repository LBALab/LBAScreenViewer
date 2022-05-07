------------------------------------------------------------------------------------
LBA Screen Viewer for Little Big Adventure 1 & 2 images and movies.

Version: 1.6
Released: 08.08.2007
Author: Alexandre Fontoura [alexfont] - alexandrefontoura@oninetspeed.pt
------------------------------------------------------------------------------------

Sorry for this short description but I think just few thigs need to be talk.

[ Index ] --------------------------------------------------------------------------
------------------------------------------------------------------------------------

1 - Open Images
  1.1 - Open LBA Resource (*.hqr, *.ile)
  1.2 - Load LBA Image (*.lim)
  1.3 - Load LBA Brick/Sprite (*.brk, *.lsp)
  1.4 - Load LBA2 Raw Sprite (*.lsr)
  1.5 - Load LBA2 Saved Game Image (*.lba) 
2 - Convert BMPs to FLA Movies
3 - Import
4 - Export
5 - View
  5.1 - Full Screen
  5.2 - Settings
6 - Other things
7 - Things TODO
8 - Special Thanks
9 - Bug Report


[1] [ Open Images ] ----------------------------------------------------------------
------------------------------------------------------------------------------------

We have in this program 4 menu sections to open LBA images:
 - Open LBA Resource (*.hqr, *.ile)
 - Load LBA Image (*.lim)
 - Load LBA Brick/Sprite (*.brk, *.lsp)
 - Load LBA2 Raw Sprite (*.lsr)
 - Load LBA2 Saved Game Image (*.lba)


[1.1] [Open LBA Resource]-----------------------------

Can open all HQR and ILE that contains images inside.
You just need to open one of then and the first entry will automaticaly be displayed.
To show the others you just need to select the entry number and click in the Draw 
button or if the Automatic_Draw check box is active you just need to select the entry.


[1.2] [Load LBA Image (*.lim)]------------------------

Can open a single lim. If the palette is already opened the image will be well dsplayed.
If the palette isn't loaded you can easly drag the palette into the program.


[1.3] [Load LBA Brick/Sprite (*.brk, *.lsp)]----------

Can open bricks and sprites from both games.

A popup message will ask you if you want to use LBA2 main palette. Then you just need
to open the brick/sprite file.


[1.4] [Load LBA2 Raw Sprite (*.lsr)]------------------

Can open LBA2 raw sprites.
You just need to open the *.lsp.


[1.5] [Load LBA2 Saved Game Image (*.lba)]------------

Can open LBA2 saved games.
You just need to open the *.lba.


[2] [ Convert BMPs to FLA Movies ] ----------------------------------------------------------------------
-------------------------------------------------------------------------------------

As a cortesy of Spatarel Dan Constantin [MGLC], you can now create your own FLA Movies
and use it in-game. This little help will guide you to make this happens.

In this help a term of Keyframe will be use, so here's the explanation of it. Keyframes
are main movie frames in the movie sequence which have a "main palette". What I mean is
if we create a keyframe, for example the frame number 50, the palette of the rest frames
(deltaframes) will use this same palette. So this mean, you have to be careful, about 
the palette the deltaframes will use acording with the keyframe.

Note this converter only support 256 BMP colors, not other format.

With the converter windown opened, you must first select the BMP files sequence folder.
All the images in the folder must have the same prefix starting from 0. For example
Twinsen0.bmp, Twinsen1.bmp, Twinsen87.bmp, and so on.
Next you must mention how many number of frames you will use to create the movie. What
you need to do is simply count all the bmp images and post the number there.
To add addicional keyframes (the first frame its always a keyframe), you must put between
spaces all the keyframes you deserve, like "5 34 44 100 130". All the frames between
two keyframes will be treated as a deltaframe as explained above.

You can or not include samples in the movie. This samples are inside flasamp.hqr file.
Note this feature wasn't tested much and it could not work yet as deserve. So the
"Sample Index" will be the hqr sample index, the "Sample Frequence" the frequnce the
engine will play that sound and "Sample Repeat" to know how many times you deserve to
repeate that sample.

Hope this little FAQ helps you, and all bug reports will be appreciate.


[3] [ Import ] ----------------------------------------------------------------------
-------------------------------------------------------------------------------------

You can import:
 - LBA Image (*.lim)
 - LBA Palette (*.pal)
 - Bitmap (*.bmp)
 - Jpeg (*.jpg)
 - GIF (*.gif)
 - PNG (*.png)
 - PCX (*.pcx)
 - Raw Palette (*.raw)


[4] [ Export ] ----------------------------------------------------------------------
-------------------------------------------------------------------------------------

You can export:
 - LBA Image (*.lim)
 - LBA Palette (*.pal)
 - Bitmap (*.bmp)
 - Jpeg (*.jpg)
 - GIF (*.gif)
 - PNG (*.png)
 - PCX (*.pcx)
 - Raw Palette (*.raw)


[5] [ View ] ----------------------------------------------------------------
------------------------------------------------------------------------------------

[5.1] [Full Screen]-----------------------------

You can access the Full Screen option by clicking in the submenu View->Full Screen. 
Also you can access throught the shortcut key, CTRL+F.

If an High Qualit Resource file (*.HQR) is opened and you'r seeng in Full Screen mode
you can change resource indexs by clicking in the shortcut keys "+" and "-".

[5.2] [Settings]--------------------------------

Here you can set the file associations. For default, I already set the ones I think its
most necessary for this program, anyway you can always unset ;).


[6] [ Other Things ] -----------------------------------------------------------------
-------------------------------------------------------------------------------------

 - When you open a lim file without palette in the first time, the image will be 
   displayed wrongly because the missing palette. Then if you open an image with the
   palette or juat the palette itself, a temporary palette would be saved in the
   program directory. That way you when you open an lim file without the palette the
   program will search for this temporary palette and will open the image with it.


[7] [ Things TODO ] -----------------------------------------------------------------
-------------------------------------------------------------------------------------

 - Feature to save LBA1 sprites. (not a priority right now)
 - Feature to open the LBA2 SMK movies.
 (help is welcome)


[8] [ Special Thanks ] --------------------------------------------------------------
-------------------------------------------------------------------------------------

 - Kazimierz Król [Zink]. He help me so much that I can't describe.
 - Spatarel Dan Constantin [MGLC] for making the BMP to FLA converter and allow me to
   integrate in Screen Viewer.
 - Some ideas/codes taken from the opensource TwinEngine by Vicent Hamm [yaz0r].
 - Palette display idea taken from the Michiel Hendriks [El Muerte] LBA Viewer.
 - For all people that report bugs and help me to improve this program.
 - Jakob Alminde [CrazyBee]. Because he want to be in the credit list :D


[9] [ Bug Report ] ------------------------------------------------------------------
-------------------------------------------------------------------------------------

 - Ido Beeri [ChaosFish]
 - Kazimierz Król [Zink]
 - João de Camões [OBrasilo]
 - [LBAWinOwns]