program test_fort

    implicit none

    integer*8 nmax
    parameter(nmax = 100000)     !! F90 allows dynamic allocation
    double precision x(nmax,nmax)
    double precision xrow(nmax)
  
    integer*8 n,i,j,l,loop_count
    INTEGER(KIND=4) :: prog_start,prog_end,start,end,rate
    REAL(KIND=4) :: time
   
    call system_clock(COUNT_RATE=rate)
    call system_clock(COUNT=prog_start)
    n = 10000
    loop_count = 1000
    call setx(n,x(1:n,1:n))
    print '("matrix size : ", I0," x ", I0)',n ,n
    print '("Number of loops: ", I0)',loop_count
    !!do i = 1,n
    !!    write(6,100) (x(i,j),j=1,n)
    !!enddo
    !!write(6,*) ' '




    !! Easy because data is contiguous in i
    j = 2
    call system_clock(COUNT=start)
    do l = 1,loop_count
       call setcol(n,x(:,j))
    enddo 
    call system_clock(COUNT=end)
    time = REAL((end-start)*1000.0/rate,4)
    print '("set column time : ",f12.4," ms.")',time
    !!do i = 1,n
    !!    write(6,100) (x(i,j),j=1,n)
    !!enddo
    !!write(6,*) ' '

    !! Expensive, since entries in rows are strided by n
    !! This uses F90 "slices". 
    call system_clock(COUNT=start)
    i = 3
    do l = 1,loop_count
       do j = 1,n
          xrow(j) = x(j,i)   !!! claw pack does this to get a fast direction
       enddo
    enddo
    call system_clock(COUNT=end)
    time = REAL((end-start)*1000.0/rate,4)
    print '("set row time old : ",f12.4," ms.")',time
    !!call setrow(n,xrow)
    call system_clock(COUNT=start)
    do i = 1,loop_count
       call setrow(n,x(i,:))
    enddo
    call system_clock(COUNT=end)
    time = REAL((end-start)*1000.0/rate,4)
    print '("set row time slice method: ",f12.4," ms.")',time
    !!do i = 1,n
    !!    write(6,100) (x(i,j),j=1,n)
    !!enddo
    call system_clock(COUNT=prog_end)
    time = REAL((prog_end-prog_start)*1000.0/rate,4)
    print '("overall program runtime : ",f12.4," ms.")',time

100 format(20F6.0)


end program test_fort


subroutine setx(n,x)
    implicit none

    integer*8 n
    double precision x(n,n)

    integer i,j

    do i = 1,n
        do j = 1,n
            x(i,j) = 10
        enddo
    end do

end subroutine setx

subroutine setcol(n,xcol)
    implicit none


    integer*8 n
    double precision xcol(n)  !! Assumes indexing 1,2,3,...,n
    integer i

    do i = 1,n
        xcol(i) = 5
    end do


end subroutine setcol


subroutine setrow(n,xrow)
    implicit none


    integer*8 n
    double precision xrow(n)

    integer j

    do j = 1,n
        xrow(j) = 7
    end do


end subroutine setrow
