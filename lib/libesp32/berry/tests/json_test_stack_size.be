# Test JSON parsing with large objects (stack size test)
import json

# Create large JSON object to test stack handling
arr = "{"
for i : 0..1000
    arr += '"k' + str(i) + '": "v' + str(i) + '",'
end
arr += "}"

json.load(arr)  # Should not cause stack overflow
