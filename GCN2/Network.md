# Pytorch model specifications

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
