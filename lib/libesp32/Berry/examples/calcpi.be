def cpi(n)
    i = 2
    pi = 3
    while i <= n
        term = 4.0 / (i * (i + 1) * (i + 2))
        if i % 4
            pi = pi + term
        else
            pi = pi - term
        end
        i = i + 2
    end
    return pi
end

print("pi =", cpi(100))
