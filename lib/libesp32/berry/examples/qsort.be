def qsort(data)
    # do once sort
    def once(left, right)
        var pivot = data[left] # use the 0th value as the pivot
        while left < right # check if sort is complete
            # put the value less than the pivot to the left
            while left < right && data[right] >= pivot
                right -= 1 # skip values greater than pivot
            end
            data[left] = data[right]
            # put the value greater than the pivot on the right
            while left < right && data[left] <= pivot
                left += 1 # skip values less than pivot
            end
            data[right] = data[left]
        end
        # now we have the index of the pivot, store it
        data[left] = pivot
        return left # return the index of the pivot
    end
    # recursive quick sort algorithm
    def _sort(left, right)
        if left < right # executed when the array is not empty
            var index = once(left, right) # get index of pivot for divide and conquer
            _sort(left, index - 1) # sort the data on the left
            _sort(index + 1, right) # sort the data on the right
        end
    end
    # start quick sort
    _sort(0, data.size() - 1)
    return data
end

import time, math
math.srand(time.time()) # sse system time as a random seed
data = []
# put 20 random numbers into the array
for i : 1 .. 20
    data.push(math.rand() % 100)
end
# sort and print
print(qsort(data))
