main:
      li  s2, 20
      li  s1, 30
      li  s0, 0	# the product
whileLoop:
    	beq s1, x0, endWhile
    	andi t0, s1, 0x01
    	beq t0, x0, next
    	# if lsb is 1:
     	add s0, s0, s2
next:
    	sll s2, s2, 1
    	srl s1, s1, 1
    	j whileLoop
endWhile:
