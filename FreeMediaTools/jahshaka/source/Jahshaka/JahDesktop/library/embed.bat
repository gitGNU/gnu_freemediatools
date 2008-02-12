@echo on
set FILES=trash.png tick-mini.png up_folder_d.png up_folder.png
set FILES=%FILES% search-mini.png collection-iconc.png moviemedia.png
set FILES=%FILES% minidb-16w-add.png minidb-16w.png add.png  
set FILES=%FILES% arrowleft_green_16_d.png arrowleft_green_16_h.png

qembed --images %FILES% > embedded-images.h
