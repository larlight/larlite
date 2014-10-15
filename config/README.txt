############################
#                          #
# README for config script #
#                          #
############################

This directory include:

  (*) set of config shell scripts
  (*) set of utility python scripts under "python" directory
  (*) set of template files under "template" directory

Only config shell scripts are relevant for users and they are described below.

The provided config scripts help some basic operations including:

  (a) set up environment for using LArLight
  (b) set up environment for using mrb + LArSoft + uboone offline tools (job submission scripts, etc)
  (c) checkout (download) local mrb repository for LArSoft development
  (d) set up environment for using (b)
  (e) checkout (download) local mrb repository for LArSoft modules that is relevant for LArLight

Among these operations, (c) and (e) are one-time-only operation unless you want to create a new set up.
On the other hand (a), (b) and (d) are per-log-in operation, unless one is switching a local repository in the same session.
The usage of these scripts are described below.

(a) setup.sh ... needs $LARLITE_BASE_DIR to be pointing to LArLight directory.

    > source $LARLITE_BASE_DIR/config/setup.sh

(b) setup_mrb_lar.sh

    > source $LARLITE_BASE_DIR/config/setup_mrb_lar.sh

(c) checkout_mrb_local.sh ... let me label the local repository to be $YOUR_REPO in this example.

    > source $LARLITE_BASE_DIR/config/checkout_mrb_local.sh  $YOUR_REPO

(d) setup_mrb_local.sh ... again I use $YOUR_REPO as an example exporession of your local repository

    > source $LARLITE_BASE_DIR/config/setup_mrb_local.sh $YOUR_REPO

(e) checkout_mrb_larlight.sh

    > source $LARLITE_BASE_DIR/config/checkout_mrb_larlight.sh

Any questions? Ask kazuhiro@nevis.columbia.edu

Kazu


