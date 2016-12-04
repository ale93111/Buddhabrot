def plot(hitcount,w,h):
    import numpy as np
    import matplotlib.pyplot as plt
    path = "./"
    
    phitcount = np.array(hitcount)
    img = np.reshape(phitcount, (w,h,3))
    img = np.array(img)
    img = img.astype(float)/65536
    
    print img.dtype
    #plt.figure(figsize=(9, 9))
    #plt.imshow(img)
    plt.imsave(path+"b.png",img)
    
    return 0