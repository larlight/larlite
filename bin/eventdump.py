import sys
try:
    from ROOT import larlite as fmwk
except ImportError:
    print
    print "You don't seem to have larlite library available for PyROOT..."
    print "Exiting..."
    print
    sys.exit(1)

user_ctr = -1
if len(sys.argv) == 3:
    try:
        user_ctr = int(sys.argv[2])
    except ValueError:
        print 'Error: 2nd argument must be an integer!'
        sys.exit(1)

mgr = fmwk.storage_manager()
mgr.add_in_filename(sys.argv[1])
mgr.set_io_mode(mgr.kREAD)
mgr.open()

if not mgr.is_open():
    print
    print 'Failed to open an input file:',sys.argv[1]
    print
    sys.exit(1)

FORMAT_VEC="\033[95mProduct\033[00m \"%10s \" (enum=%2d) ...\033[95m made by\033[00m %20s ...\033[95m count :\033[00m %d "
FORMAT="\033[95mProduct\033[00m \"%10s \" (enum=%2d) ...\033[95m made by\033[00m %20s ...\033[95m count :\033[00m 1 "

ctr=0;
while mgr.next_event():

    product_id_v = mgr.list_input_product()
    print "\033[93mEntry:\033[00m",ctr
    for x in xrange(product_id_v.size()):

        product_id  = product_id_v.at(x)
        product_ptr = mgr.get_data(product_id.first,product_id.second)
        if product_ptr :
            try:
                print FORMAT_VEC % (mgr.product_name(product_id.first),product_id.first,product_id.second,product_ptr.size())
            except AttributeError:
                print FORMAT % (mgr.product_name(product_id.first),product_id.first,product_id.second)
    print
    ctr+=1
    if user_ctr > 0 and user_ctr < ctr: break
