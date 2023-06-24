import os
import json



def assert_load_failed(text)
    assert(json.load(text) == nil)
end

var input_file = open("tests/json_test_cases.json", "r")
var test_cases = json.load(input_file.read())

# check positive cases
var has_failed_positives = false
for case_name : test_cases["positive"].keys()
   var case = test_cases["positive"][case_name]
   var val = json.load(case)
   if val == nil && case != "null"
       print("Failed to load case: " + case_name)
       has_failed_positives = true
   end
end

if has_failed_positives
    assert(false)
end

# check negative cases

var has_failed_negatives = false
for case_name : test_cases["negative"].keys()
   var case = test_cases["negative"][case_name]
   
   var val = json.load(case)
   if val != nil
       print("Failed to fail case: " + case_name + ", got: " + json.dump(val))
       has_failed_negatives = true
   end
end

if has_failed_negatives
    # assert(false)
end

# check "any" cases, only for crashes

for case_name : test_cases["any"].keys()
    var case = test_cases["any"][case_name]
    var val = json.load(case)
end

