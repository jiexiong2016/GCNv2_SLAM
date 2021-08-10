# Pytorch model specifications

original models available at: https://drive.google.com/file/d/1MJMroL5-tl0b9__-OiCfxFP9K6X8kvTT/view

## GCNv2

```
class GCNv2(torch.nn.Module):
    def __init__(self):
        super(GCNv2, self).__init__()
        self.elu = torch.nn.ELU(inplace=True)

        self.conv1 = torch.nn.Conv2d(1, 32, kernel_size=4, stride=2, padding=1)
        self.conv2 = torch.nn.Conv2d(32, 64, kernel_size=4, stride=2, padding=1)

        self.conv3_1 = torch.nn.Conv2d(64, 128, kernel_size=3, stride=1, padding=1)
        self.conv3_2 = torch.nn.Conv2d(128, 128, kernel_size=4, stride=2, padding=1)

        self.conv4_1 = torch.nn.Conv2d(128, 256, kernel_size=3, stride=1, padding=1)
        self.conv4_2 = torch.nn.Conv2d(256, 256, kernel_size=4, stride=2, padding=1)

        # Descriptor
        self.convF_1 = torch.nn.Conv2d(256, 256, kernel_size=3, stride=1, padding=1)
        self.convF_2 = torch.nn.Conv2d(256, 256, kernel_size=1, stride=1, padding=0)

        # Detector
        self.convD_1 = torch.nn.Conv2d(256, 256, kernel_size=3, stride=1, padding=1)
        self.convD_2 = torch.nn.Conv2d(256, 256, kernel_size=1, stride=1, padding=0)

        self.pixel_shuffle = torch.nn.PixelShuffle(16)

    def forward(self, x):

        x = self.elu(self.conv1(x))
        x = self.elu(self.conv2(x))

        x = self.elu(self.conv3_1(x))
        x = self.elu(self.conv3_2(x))

        x = self.elu(self.conv4_1(x))
        x = self.elu(self.conv4_2(x))

        # Descriptor xF
        xF = self.elu(self.convF_1(x))
        desc = self.convF_2(xF)
        dn = torch.norm(desc, p=2, dim=1) # Compute the norm.
        desc = desc.div(torch.unsqueeze(dn, 1)) # Divide by norm to normalize.

        # Detector xD
        xD = self.elu(self.convD_1(x))
        det = self.convD_2(xD).sigmoid()
        det = self.pixel_shuffle(det)
        return desc, det
```
## GCNv2_tiny


```
class GCNv2_tiny(torch.nn.Module):
    def __init__(self):
        super(GCNv2_tiny, self).__init__()
        self.elu = torch.nn.ELU(inplace=True)

        self.conv1 = torch.nn.Conv2d(1, 32, kernel_size=4, stride=2, padding=1)
        self.conv2 = torch.nn.Conv2d(32, 32, kernel_size=4, stride=2, padding=1)

        self.conv3_1 = torch.nn.Conv2d(32, 64, kernel_size=3, stride=1, padding=1)
        self.conv3_2 = torch.nn.Conv2d(64, 64, kernel_size=4, stride=2, padding=1)

        self.conv4_1 = torch.nn.Conv2d(64, 128, kernel_size=3, stride=1, padding=1)
        self.conv4_2 = torch.nn.Conv2d(128, 128, kernel_size=4, stride=2, padding=1)

        # Descriptor
        self.convF_1 = torch.nn.Conv2d(128, 256, kernel_size=3, stride=1, padding=1)
        self.convF_2 = torch.nn.Conv2d(256, 256, kernel_size=1, stride=1, padding=0)

        self.convD_1 = torch.nn.Conv2d(128, 256, kernel_size=3, stride=1, padding=1)
        self.convD_2 = torch.nn.Conv2d(256, 256, kernel_size=1, stride=1, padding=0)

        self.pixel_shuffle = torch.nn.PixelShuffle(16)

    def forward(self, x):

        x = self.elu(self.conv1(x))
        x = self.elu(self.conv2(x))

        x = self.elu(self.conv3_1(x))
        x = self.elu(self.conv3_2(x))

        x = self.elu(self.conv4_1(x))
        x = self.elu(self.conv4_2(x))

        # Descriptor xF
        xF = self.elu(self.convF_1(x))
        desc = self.convF_2(xF)
        dn = torch.norm(desc, p=2, dim=1) # Compute the norm.
        desc = desc.div(torch.unsqueeze(dn, 1)) # Divide by norm to normalize.

        # Detector xD
        xD = self.elu(self.convD_1(x))
        det = self.convD_2(xD).sigmoid()
        det = self.pixel_shuffle(det)
        return desc, det
```

## GCNV2_Mobile(unstable)
Mobilenet-v2 encoder is from: https://github.com/tonylins/pytorch-mobilenet-v2/blob/master/MobileNetV2.py

```

def conv_bn(inp, oup, stride):
    return nn.Sequential(
        nn.Conv2d(inp, oup, 3, stride, 1, bias=False),
        nn.BatchNorm2d(oup),
        nn.ReLU6(inplace=True)
    )
    
class GCNv2_Mobile(torch.nn.Module):
    def __init__(self):
        super(GCNv2_Mobile, self).__init__()
        
        # Use Mobilenet-v2 as encoder and replace first layer to take gray image.
        mobile_net = MobileNetV2(n_class=1000)
        
        # Optional, load pre-trained weights.
        # state_dict = torch.load('mobilenet_v2.pth.tar')
        
        mobile_net.load_state_dict(state_dict)
        
        mobile_net.features[0] = nn.Sequential(
                            nn.Conv2d(1, 32, 3, 2, 1, bias=False),
                            nn.BatchNorm2d(32),
                            nn.ReLU6(inplace=True)
                            )

        self.base = mobile_net.features[:-1]
        
        # Also use ReLU6 for other layers
        
        # Descriptor
        self.convF_1 = conv_bn(320, 256, 1)
        self.convF_2 = torch.nn.Conv2d(256, 256, kernel_size=1, stride=1, padding=0)

        # Detector
        self.convD_1 = conv_bn(320, 256, 1)
        self.convD_2 = torch.nn.Conv2d(256, 256, kernel_size=1, stride=1, padding=0)

        self.pixel_shuffle = torch.nn.PixelShuffle(16)
        
    def forward(self, x):
        
        # Mobilenet encoder
        x = self.base(x)
       
        # Descriptor xF
        xF = self.elu(self.convF_1(x))
        desc = self.convF_2(xF)
        dn = torch.norm(desc, p=2, dim=1) # Compute the norm.
        desc = desc.div(torch.unsqueeze(dn, 1)) # Divide by norm to normalize.

        # Detector xD
        xD = self.elu(self.convD_1(x))
        det = self.convD_2(xD).sigmoid()
        det = self.pixel_shuffle(det)
        return desc, det
```
