Step 1: compile the read_nGEM_data.cpp

% make

Step 2. run the code:

% ./read_nGEM_data GEM000030_20220321/GEM000030_00_000_000.edb | tee test.out

==> the code generates the ascii file data_for_root.txt

Step 3.  use ROOT to create the data tree 

% root -l

		root[0].L CreateGEMTree.C  
		root[1] CreateGEMTree()

		 ..... it does something ...
 
		root[3] .q

Step 4. use ROOT to plot the data:

% root -l ngem.root

	    root[0] .x plot_nGEM.C
	        
	         ....it plots stuff ....

