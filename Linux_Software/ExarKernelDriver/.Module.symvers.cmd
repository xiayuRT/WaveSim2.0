cmd_/home/wave/Desktop/Linux_Software/ExarKernelDriver/Module.symvers :=  sed 's/ko$$/o/'  /home/wave/Desktop/Linux_Software/ExarKernelDriver/modules.order | scripts/mod/modpost -m -a    -o /home/wave/Desktop/Linux_Software/ExarKernelDriver/Module.symvers -e -i Module.symvers -T - 
