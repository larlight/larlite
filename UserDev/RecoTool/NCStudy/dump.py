import os,sys

import pandas as pd
import root_numpy as rn


df = pd.DataFrame(rn.root2array("from_test_ana_you_can_remove_me.root"))
print df.iloc[int(sys.argv[1])]
