# interesting driver quirks
Study bits from working on linux drivers

## atomics

- atomic_t is a signed int, so the usable range is INT_MIN to INT_MAX, there is no unsigned version of an atomic_t
- there is no longer a 24 bit limitation
- if I need a pointer sized atomic use atomic_long_t

## publishing a pointer to other CPUs
- xchg(&ptr,new)  and   cmpxchg(&ptr,old, new) these  are lock free


## current reading pointers with , occasional updates

The linux way of hot read path swapping ina new buffer

- rcu_dereference() , rcu_assign_pointer() 






