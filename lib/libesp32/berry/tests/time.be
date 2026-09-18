import time

# time.time() returns current unix timestamp as int
var t = time.time()
assert(type(t) == 'int')
assert(t > 0)

# time.clock() returns elapsed CPU time as real
var c = time.clock()
assert(type(c) == 'real')
assert(c >= 0)

# time.dump() with a known epoch: 2021-01-01 00:00:00 UTC = 1609459200
var ts = 1609459200
var d = time.dump(ts)
assert(isinstance(d, map))
assert(d['year'] == 2021)
assert(d['month'] == 1)
assert(d['epoch'] == ts)

# all expected keys are present
assert(d.contains('hour'))
assert(d.contains('min'))
assert(d.contains('sec'))
assert(d.contains('day'))
assert(d.contains('weekday'))

# weekday is 0 (Sunday) through 6 (Saturday)
assert(d['weekday'] >= 0 && d['weekday'] <= 6)

# time.dump() with no argument returns nil (branch: be_top < 1)
assert(time.dump() == nil)

# time.dump() with a non-int argument returns nil (branch: !be_isint)
assert(time.dump('hello') == nil)
assert(time.dump(3.14) == nil)

# time.dump() with another known timestamp: 2000-01-01 00:00:00 UTC = 946684800
var d2 = time.dump(946684800)
assert(isinstance(d2, map))
assert(d2['year'] == 2000)
assert(d2['epoch'] == 946684800)

# time.dump() with epoch 0 = 1970-01-01 00:00:00 UTC
var d0 = time.dump(0)
assert(isinstance(d0, map))
assert(d0['epoch'] == 0)
assert(d0['weekday'] >= 0 && d0['weekday'] <= 6)

# time.time() result is consistent with time.dump()
var now = time.time()
var nd = time.dump(now)
assert(nd['year'] >= 2024)
assert(nd['month'] >= 1 && nd['month'] <= 12)
assert(nd['day'] >= 1 && nd['day'] <= 31)
assert(nd['hour'] >= 0 && nd['hour'] <= 23)
assert(nd['min'] >= 0 && nd['min'] <= 59)
assert(nd['sec'] >= 0 && nd['sec'] <= 59)
assert(nd['weekday'] >= 0 && nd['weekday'] <= 6)
