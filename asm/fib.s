        .globl	_Z3fibi
        .type	_Z3fibi, @function
_Z3fibi:
        # No need to save and restore ESP and EBP,
        # since only EAX, ECX, and EDX are used.
        # Put first argument in EAX.  This is the iteration count.
        movl	%eax, -4(%rsp)
        # ECX and EDX are consecutive elements of the Fibonacci sequence.
        # Initialize them to (0, 1).  Return value will be in EDX.
        xor     %ecx, %ecx
        movl    $1, %edx
        # Iterate.
it:
        # If no more iterations, return.
        cmp     $0, %eax
        je      ret
        # (CX, DX) <= (CX + DX, DX)
        add     %edx, %ecx
        # (CX, DX) <= (DX, CX)
        xor     %edx, %ecx
        xor     %ecx, %edx
        xor     %edx, %ecx
        # Account for one iteration completed.
        dec     %eax
        jmp     it
ret:
        movl    %edx, %eax
        ret
