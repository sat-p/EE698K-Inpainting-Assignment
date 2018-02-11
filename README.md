Dependancies:
    * OpenCV
    * numpy
    * scipy
    * skimage
    
Instructions:
    - Compile code by executing 'make bin' in main directory.
    - Two binary files ./code/app/src/main.out and ./code/tools/src/create_dictionary
      should appear
    - run './code/tools/src/create_dictionary 5 ./photos' to create the dictionary.
    - run './code/src/app/main.out' with appropriate arguments to perform a specific
      experiment.
      Example :
      './code/app/src/main.out 1e-3 256' for running OMP with
        error threshold = 1e-5
        dictionary size = 256
        
      './code/app/src/main.out 1e-3 256 any_text' for running IRLS with
        error threshold = 1e-5
        dictionary size = 256
    or
      run 'python script.py' to generate a plot comparing the NIQE (Naturalness
      Image Quality Evaluator).
      
      All commands are to be perfomed from the main directory.
      Code was tested on an Ubuntu 16.04 computer.
      Directies photos, data, patch need to exist in the main directory for code to
      work.
      
Description of files:
    - code/app : Contains the main file which returns Inpainted Image after
                 calling EE698K::tools::sparse_inpaint()
    - code/tools : Contains EE698K::tools::sparse_inpaint() (driver function),
                   niqe.py (a Python script ot evaluate NIQE of an image)
    - code/algos : Contains EE698K::alogos::SparseInpaint (Class which performs the
                   inpainting), omp (Orthogonal Matching Pursuit), irls (Iterative
                   Reweighted Least Squares)
    - script.py : Script to perform comparsion after compiling C++ code.
        