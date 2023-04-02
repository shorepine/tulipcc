import tulip
tulip.gpu_reset()
text = "The quick brown fox loves to use his lazy Tulip"
for i in range(19):
    tulip.str("#%d: %s" % (i,text), 20, 20+(i*30), 255, i)

