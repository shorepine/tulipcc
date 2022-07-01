# Sequencer
import machine


ppq = 48
bpm = 120.0
swing = 0
tick = 0
beat = 0
#last_us = time.ticks_us()
playing = 0
seq_id = 0
loop_length = 0 # if <= 0, don't loop, otherwise loop this many beats
sequence = {} 
timer = machine.Timer(0)  

def handleInterrupt(timer):
	global tick, beat, ppq, loop_length
	tick = tick + 1
	if(tick == ppq):
		beat = beat + 1
		tick = 0
		if((loop_length > 0) and (beat == loop_length)):
			beat = 0
	for (method, args) in sequence.get(beat, {}).get(tick, []):
		method(*args)


def set_bpm(in_bpm):
	global bpm, us_per_tick, ppq
	bpm = in_bpm
	us_per_tick = int(1000000.0 / ((float(bpm)/60.0) * float(ppq)))
	timer.period(int(us_per_tick/1000.0))

#def tick_thread():
#	global us_per_tick, last_us, tick, beat, ppq, loop_length
#	while 1: # TODO, probably not spin the CPU if possible... interrupt? timer? think
#		if(time.ticks_diff(time.ticks_us(), last_us) > us_per_tick):
#			last_us = time.ticks_us()
#			tick = tick + 1;
#			if(tick == ppq): 
#				beat = beat + 1
#				tick = 0
#				if((loop_length > 0) and (beat == loop_length)):
#					beat = 0
#			for (method, args) in sequence.get(beat, {}).get(tick, []):
#				method(*args)

def stop():
	#global playing, seq_id
	global playing
	if(playing):
		timer.stop()
#		_thread.kill(seq_id)
		playing = 0

def start():
	#global playing, seq_id, us_per_tick
	global playing, us_per_tick
	if(not playing):
		playing = 1
		timer.init(period=int(us_per_tick/1000.0), mode=machine.Timer.PERIODIC, callback=handleInterrupt)

	#if(not playing):
	#	playing = 1
	#	# Kick off tick thread
	#	seq_id = _thread.start_new_thread("sequencer", tick_thread, ())

def seek(in_beat, in_tick=0):
	global beat, tick
	beat = in_beat
	tick = in_tick

def schedule(method, args, beat, tick=0):
	# Basically, add a prickle method (with params) to a list keyed to a tick/beat.
	# When tick/beat comes around, call the prickle calls in the list
	# TOOD: defaultdict? Better way? Not sure needed
	sequence[beat] = sequence.get(beat, {})
	sequence[beat][tick] = sequence[beat].get(tick, [])
	sequence[beat][tick].append((method, args))

set_bpm(bpm)

"""
| _thread.start_new_thread("th_name", th_func, args[, kwargs]) | Start a new thread and return its identifier. The thread executes the function with the argument list args (which must be a tuple). The optional kwargs argument specifies a dictionary of keyword arguments. |
| _thread.stack_size([size]) | Return the thread stack size (in bytes) used when creating new threads. The optional size argument specifies the stack size to be used for subsequently created threads. The maximum stack size used by the thread can be checked with *_thread.list()* |
| _thread.allowsuspend(allow) | The thread can be suspended if *allow* is True. When created, thread does not allow suspension until explicitly set by this function. **The method must be called from the thread function**. |
| _thread.suspend(th_id) | Suspend the execution of the thread function. |
| _thread.resume(th_id) | Resume the execution of the thread function previously suspended with *_thread.suspend()*. |
| _thread.kill(th_id) | Terminate the thread, free all allocated memory. |
| _thread.getThreadName(th_id) | Get the name of the thread whose id is *th_id*. |
| _thread.getSelfName() | Get the name of the thread executing this method. |
| _thread.getReplID() | Get the thread id of the main (repl) thread. Can be uset to send notifications/messages to the main thread. |
| _thread.getnotification() | Check if any notification was sent to the thread executing this method. Returns integer >0 if there was pending notification or 0 if not |
| _thread.getmsg() | Check if any message was sent to the thread executing this method. Returns 3-items tuple containing message type, sender thread id and message itself (integer or string). |
| _thread.notify(th_id, value) | Send notification to the thread with id *th_id*. *value* is integer >0. |
| _thread.sendmsg(th_id, msg) | Send message to the thread with id *th_id*. *msg* can be integer or string. |
| _thread.replAcceptMsg([flag]) | Return True if the main thread (repl) is allowed to accept messages. If executed from the main thread, optional *flag* (True|False) argument can be given to allow/dissallow accepting messages in the main thread |
| _thread.list([print]) | Print the status of all created threads. If the optional *print* argument is set to *False*, returns the tuple with created threads information. Thread info tuple has *th_id*, *type*, *name*, *suspended state*, *stack size* and *max stack used* items |
"""
"""
import machine
 
interruptCounter = 0
totalInterruptsCounter = 0
 
timer = machine.Timer(0)  
 
def handleInterrupt(timer):
  global interruptCounter
  interruptCounter = interruptCounter+1
 
timer.init(period=1000, mode=machine.Timer.PERIODIC, callback=handleInterrupt)
 
while True:
  if interruptCounter>0:
    state = machine.disable_irq()
    interruptCounter = interruptCounter-1
    machine.enable_irq(state)
 
    totalInterruptsCounter = totalInterruptsCounter+1
    print("Interrupt has occurred: " + str(totalInterruptsCounter))
"""

