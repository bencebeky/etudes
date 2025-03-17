from akamaster import ResNet20
from hubert10 import HubertResNet18
from kamal import ResNet9
from torch import nn, Tensor
from torch.utils.data import DataLoader
from torchvision import transforms
import numpy as np
import plotext as plt
import torch
import torchvision

device = "cuda" if torch.cuda.is_available() else "cpu"
batch_size = 64

# Every neural network tested here starts with a conv2d with bias, and no
# pretrained weights are used, therefore mean and std values do not matter.
stats = ((0.0, 0.0, 0.0), (1.0, 1.0, 1.0))
dataset_train = torchvision.datasets.CIFAR10(
    root="data",
    train=True,
    download=True,
    transform=transforms.Compose(
        [
            transforms.RandomCrop(32, padding=4),
            transforms.RandomHorizontalFlip(),
            transforms.ToTensor(),
            transforms.Normalize(*stats),
        ]
    ),
)
dataset_test = torchvision.datasets.CIFAR10(
    root="data",
    train=False,
    download=True,
    transform=transforms.Compose(
        [
            transforms.ToTensor(),
            transforms.Normalize(*stats),
        ]
    ),
)

train_loader = DataLoader(
    dataset_train, batch_size=batch_size, pin_memory=True, shuffle=True
)
test_loader = DataLoader(
    dataset_test, batch_size=batch_size, pin_memory=True, shuffle=False
)


def train(model, optimizer, criterion):
    total_loss = 0
    total_correct = 0
    model.train()
    for data in train_loader:
        images, labels = data
        images = images.to(device)
        labels = labels.to(device)
        optimizer.zero_grad()
        outputs = model(images)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()

        total_loss += loss.item()
        _, predictions = torch.max(outputs.data, 1)
        total_correct += (predictions == labels).sum().item()

    return total_loss / len(train_loader), 100 * total_correct / len(
        train_loader.dataset
    )


def test(model, optimizer, criterion):
    total_loss = 0
    total_correct = 0
    model.eval()
    with torch.no_grad():
        for data in test_loader:
            images, labels = data
            images = images.to(device)
            labels = labels.to(device)
            outputs = model(images)
            loss = criterion(outputs, labels)

            total_loss += loss.item()
            _, predictions = torch.max(outputs.data, 1)
            total_correct += (predictions == labels).sum().item()
    return total_loss / len(test_loader), 100 * total_correct / len(test_loader.dataset)


# model = HubertResNet18(num_classes=10)
# model = torchvision.models.resnet18(num_classes=10)
# model = resnet20(num_classes=10)
# model = ResNet9(num_classes = 10)

model = model.to(device)
optimizer = torch.optim.SGD(
    model.parameters(), lr=1e-2, momentum=0.9, weight_decay=1e-4
)
criterion = nn.CrossEntropyLoss()
lr_scheduler = torch.optim.lr_scheduler.CosineAnnealingLR(optimizer, T_max=200)

epochs = 50
train_loss = np.empty(epochs)
train_correct = np.empty(epochs)
test_loss = np.empty(epochs)
test_correct = np.empty(epochs)


for epoch in range(epochs):
    train_loss[epoch], train_correct[epoch] = train(model, optimizer, criterion)
    test_loss[epoch], test_correct[epoch] = test(model, optimizer, criterion)
    lr_scheduler.step()

    print(
        "epoch",
        epoch,
        "train_loss",
        "{:.2e}".format(train_loss[epoch]),
        "test_loss",
        "{:.2e}".format(test_loss[epoch]),
        "train_correct",
        "{:.1f}".format(train_correct[epoch]),
        "test_correct",
        "{:.1f}".format(test_correct[epoch]),
    )

    plt.clf()

    plt.subplots(1, 2)
    left = plt.subplot(1, 1)
    left.plot(train_loss[: epoch + 1], label="train")
    left.plot(test_loss[: epoch + 1], label="test")
    left.ylim()

    right = plt.subplot(1, 2)
    right.plot(train_correct[: epoch + 1], label="train")
    right.plot(test_correct[: epoch + 1], label="test")
    right.ylim(lower=None, upper=100.0)

    plt.show()
