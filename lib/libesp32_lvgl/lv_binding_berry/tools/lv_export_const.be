# use this simple script with every LVGL update
#
# generate `lv_constants.be` containing all lvgl integer constants

var f_out = open("lv_constants.be", "w")

f_out.write("# LVGL integer constants\n")
f_out.write(f"# LVGL version {lv.version}\n\n")

var cc = lv._constants()

# get keys of a map in sorted order
def k2l(m) var l=[] if m==nil return l end for k:m.keys() l.push(k) end return l end

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


var keys = qsort(k2l(cc))

for k: keys
  f_out.write(f"lv.{k} = {cc[k]}\n")
end
f_out.close()
