function parseFile()
    assert(fileName ~= nil ,"Input file not specified.")
    file = assert(io.open(fileName,"r"),"Input file not found.")

    for line in file:lines() do
        print(line)
        --_, _, test = string.find(line, "(-?%d+%.?%d*)")
       -- print("test", test)

        -- adapted from my second project.
        -- original commentary:
        --      we use _ to throw away return values (start and end index of find)
        --      %( is an escaped ( symbol    -? means 0 or 1 - sign (negative or positive)
        --      real numbers: whole part + dot (needs to be escaped) + frac. part
        --      we only keep the captures: ([+-real number],([+-real number],([+-real number])
        -- additional commentary:
        --      expanded to 6 numbers
        --      since problem statement does not specify, i made the assumption that every number is followed by a space
        _, _, hlX, hlY, hlZ, oX, oY, oZ = string.find(line, "%((-?%d+%.?%d*), (-?%d+%.?%d*), (-?%d+%.?%d*), (-?%d+%.?%d*), (-?%d+%.?%d*), (-?%d+%.?%d*)%)")
        print(hlX, hlY, hlZ, oX, oY, oZ)
        cppAddBarrier(hlX, hlY, hlZ, oX, oY, oZ)
    end
--[[
    local luaNoOfBarriers = 1
    for i = 1, luaNoOfBarriers do
        cppAddBarrier(30.0, 3.0, 0.0, -10.0, -25.0, 0.0)
    end ]]--
    return 0
end

