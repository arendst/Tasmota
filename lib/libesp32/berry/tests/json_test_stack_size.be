import json

# this test must be in a separate file, so that the stack is not expanded yet by other tests

arr = "{"
for i : 0..1000
    arr += '"k' + str(i) + '": "v' + str(i) + '",'
end
arr += "}"

json.load(arr)
