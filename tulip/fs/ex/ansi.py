# Draw all the ansi fg / bg on the TFB
tulip.display_clock(12)
for i in range(255):
    bg = 255 - i
    str = tulip.ansi_bg(bg)
    for fg in range(255):
        str += tulip.ansi_fg(fg) + chr(fg%90 + 32)
    print(str)

