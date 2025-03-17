# Based on https://github.com/hubert10/ResNet18_from_Scratch_using_PyTorch/blob/main/resnet18.py
# 11 181 642 parameters when num_classes = 10

import torch.nn as nn


class BasicBlock(nn.Module):
    def __init__(self, in_channels, out_channels, stride=1):
        super(BasicBlock, self).__init__()

        self.conv1 = nn.Conv2d(
            in_channels,
            out_channels,
            kernel_size=3,
            stride=stride,
            padding=1,
            bias=False,
        )
        self.bn1 = nn.BatchNorm2d(out_channels)
        self.relu = nn.ReLU()
        self.conv2 = nn.Conv2d(
            out_channels,
            out_channels,
            kernel_size=3,
            padding=1,
            bias=False,
        )
        self.bn2 = nn.BatchNorm2d(out_channels)
        self.downsample = (
            None
            if stride == 1
            else nn.Sequential(
                nn.Conv2d(
                    in_channels,
                    out_channels,
                    kernel_size=1,
                    stride=stride,
                    bias=False,
                ),
                nn.BatchNorm2d(out_channels),
            )
        )

    def forward(self, x):
        identity = x

        out = self.conv1(x)
        out = self.bn1(out)
        out = self.relu(out)

        out = self.conv2(out)
        out = self.bn2(out)

        out += identity if self.downsample is None else self.downsample(identity)

        out = self.relu(out)

        return out


class HubertResNet18(nn.Module):
    def __init__(self, num_classes):
        super(HubertResNet18, self).__init__()
        self.conv1 = nn.Conv2d(
            in_channels=3,
            out_channels=64,
            kernel_size=7,
            stride=2,
            padding=3,
            bias=False,
        )
        self.bn1 = nn.BatchNorm2d(64)
        self.relu = nn.ReLU()
        self.maxpool = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)

        self.layer1block1 = BasicBlock(64, 64)
        self.layer1block2 = BasicBlock(64, 64)
        self.layer2block1 = BasicBlock(64, 128, stride=2)
        self.layer2block2 = BasicBlock(128, 128)
        self.layer3block1 = BasicBlock(128, 256, stride=2)
        self.layer3block2 = BasicBlock(256, 256)
        self.layer4block1 = BasicBlock(256, 512, stride=2)
        self.layer4block2 = BasicBlock(512, 512)

        self.avgpool = nn.AdaptiveAvgPool2d((1, 1))
        self.fc = nn.Linear(512, num_classes)

    def forward(self, x):
        x = self.conv1(x)
        x = self.bn1(x)
        x = self.relu(x)
        x = self.maxpool(x)
        x = self.layer1block1(x)
        x = self.layer1block2(x)
        x = self.layer2block1(x)
        x = self.layer2block2(x)
        x = self.layer3block1(x)
        x = self.layer3block2(x)
        x = self.layer4block1(x)
        x = self.layer4block2(x)
        x = self.avgpool(x)
        x = torch.flatten(x, 1)
        x = self.fc(x)

        return x
