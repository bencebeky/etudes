// Coding problem from leetcode.com: Given two sorted arrays `nums1` and `nums2`
// of size `m` and `n` respectively, return the median.
// The overall run time complexity should be O(log (m+n)).

#include <cstddef>
#include <iostream>
#include <vector>

double findMedianSortedArrays(const std::vector<int>& nums1,
                              const std::vector<int>& nums2) {
  // One array is empty
  if (nums1.empty()) {
    if (nums2.empty()) {
      // error
      return 0.0;
    }
    if (nums2.size() % 2 == 1) {
      return nums2[nums2.size() / 2];
    } else {
      return (nums2[nums2.size() / 2 - 1] + nums2[nums2.size() / 2]) / 2.0;
    }
  }
  if (nums2.empty()) {
    if (nums1.size() % 2 == 1) {
      return nums1[nums1.size() / 2];
    } else {
      return (nums1[nums1.size() / 2 - 1] + nums1[nums1.size() / 2]) / 2.0;
    }
  }

  // Both arrays are size 1
  if (nums1.size() == 1 && nums2.size() == 1) {
    return (nums1[0] + nums2[0]) / 2.0;
  }

  const size_t total_size = nums1.size() + nums2.size();

  // The median is larger than all elements in one array
  if (total_size % 2 == 1) {
    if (nums2.size() > total_size / 2 &&
        nums1[nums1.size() - 1] <= nums2[nums2.size() - total_size / 2 - 1]) {
      return nums2[nums2.size() - total_size / 2 - 1];
    }
    if (nums1.size() > total_size / 2 &&
        nums2[nums2.size() - 1] <= nums1[nums1.size() - total_size / 2 - 1]) {
      return nums1[nums1.size() - total_size / 2 - 1];
    }
  } else {
    if (nums2.size() >= total_size / 2 &&
        nums1[nums1.size() - 1] <= nums2[nums2.size() - total_size / 2]) {
      if (nums2.size() == total_size / 2) {
        return (nums1[nums1.size() - 1] +
                nums2[nums2.size() - total_size / 2]) /
               2.0;
      } else {
        return (std::max(nums1[nums1.size() - 1],
                         nums2[nums2.size() - total_size / 2 - 1]) +
                nums2[nums2.size() - total_size / 2]) /
               2.0;
      }
    }
    if (nums1.size() >= total_size / 2 &&
        nums2[nums2.size() - 1] <= nums1[nums1.size() - total_size / 2]) {
      if (nums1.size() == total_size / 2) {
        return (nums2[nums2.size() - 1] +
                nums1[nums1.size() - total_size / 2]) /
               2.0;
      } else {
        return (std::max(nums2[nums2.size() - 1],
                         nums1[nums1.size() - total_size / 2 - 1]) +
                nums1[nums1.size() - total_size / 2]) /
               2.0;
      }
    }
  }

  // The median is smaller than all elements in one array
  if (total_size % 2 == 1) {
    if (total_size / 2 < nums2.size() && nums1[0] >= nums2[total_size / 2]) {
      return nums2[total_size / 2];
    }
    if (total_size / 2 < nums1.size() && nums2[0] >= nums1[total_size / 2]) {
      return nums1[total_size / 2];
    }
  } else {
    if (total_size / 2 < nums2.size() + 1 &&
        nums1[0] >= nums2[total_size / 2 - 1]) {
      if (total_size / 2 < nums2.size()) {
        return (std::min(nums1[0], nums2[total_size / 2]) +
                nums2[total_size / 2 - 1]) /
               2.0;
      } else {
        return (nums1[0] + nums2[total_size / 2 - 1]) / 2.0;
      }
    }
    if (total_size / 2 < nums1.size() + 1 &&
        nums2[0] >= nums1[total_size / 2 - 1]) {
      if (total_size / 2 < nums1.size()) {
        return (std::min(nums2[0], nums1[total_size / 2]) +
                nums1[total_size / 2 - 1]) /
               2.0;
      } else {
        return (nums2[0] + nums1[total_size / 2 - 1]) / 2.0;
      }
    }
  }

  // One array has size 1
  if (nums1.size() == 1) {
    if (total_size % 2 == 1) {
      if (nums1[0] <= nums2[nums2.size() / 2 - 1]) {
        return nums2[nums2.size() / 2 - 1];
      }
      if (nums1[0] >= nums2[nums2.size() / 2]) {
        return nums2[nums2.size() / 2];
      }
      return nums1[0];
    } else {
      if (nums1[0] <= nums2[nums2.size() / 2]) {
        return (std::max(nums2[nums2.size() / 2 - 1], nums1[0]) +
                nums2[nums2.size() / 2]) /
               2.0;
      } else {
        return (nums2[nums2.size() / 2] +
                std::min(nums2[nums2.size() / 2 + 1], nums1[0])) /
               2.0;
      }
    }
  }
  if (nums2.size() == 1) {
    if (total_size % 2 == 1) {
      if (nums2[0] <= nums1[nums1.size() / 2 - 1]) {
        return nums1[nums1.size() / 2 - 1];
      }
      if (nums2[0] >= nums1[nums1.size() / 2]) {
        return nums1[nums1.size() / 2];
      }
      return nums2[0];
    } else {
      if (nums2[0] <= nums1[nums1.size() / 2]) {
        return (std::max(nums1[nums1.size() / 2 - 1], nums2[0]) +
                nums1[nums1.size() / 2]) /
               2.0;
      } else {
        return (nums1[nums1.size() / 2] +
                std::min(nums1[nums1.size() / 2 + 1], nums2[0])) /
               2.0;
      }
    }
  }

  // General case

  size_t i_lower_bound = 0;
  size_t i_upper_bound = nums1.size() - 1;
  size_t j_lower_bound = 0;
  size_t j_upper_bound = nums2.size() - 1;

  const size_t target_sum = (nums1.size() + nums2.size() - 1) / 2 - 1;

  /* Perform binary search for i, j.
  Constraints during iteration:
  0 <= i_lower_bound <= i_upper_bound < nums1.size()
  0 <= j_lower_bound <= j_upper_bound < nums2.size()
  i_lower_bound + j_lower_bound <= target_sum
  i_upper_bound + j_upper_bound >= target_sum
  nums1[i_lower_bound] <= nums2[std::min(nums2.size() - 1, j_upper_bound + 1)]
  nums2[j_lower_bound] <= nums1[std::min(nums1.size() - 1, i_upper_bound + 1)]
  */

  while (i_lower_bound < i_upper_bound || j_lower_bound < j_upper_bound) {
    if (i_lower_bound + 2 <= i_upper_bound) {
      size_t i_candidate = (i_upper_bound + i_lower_bound) / 2.0;
      if (i_candidate + j_lower_bound <= target_sum &&
          nums1[i_candidate] <= nums2[std::min(nums2.size() - 1, j_upper_bound +
          1)]) {
        i_lower_bound = i_candidate;
        continue;
      }
      i_candidate = (i_upper_bound + i_lower_bound + 1) / 2.0;
      if (i_candidate + j_upper_bound >= target_sum &&
                 nums2[j_lower_bound] <= nums1[std::min(nums1.size() - 1,
                 i_candidate + 1)]) {
        i_upper_bound = i_candidate;
        continue;
      }
    }
    if (i_lower_bound + 1 == i_upper_bound) {
      size_t i_candidate = i_upper_bound;
      if (i_candidate + j_lower_bound <= target_sum &&
          nums1[i_candidate] <= nums2[std::min(nums2.size() - 1, j_upper_bound +
          1)]) {
        i_lower_bound = i_candidate;
        continue;
      }
      i_candidate = i_lower_bound;
      if (i_candidate + j_upper_bound >= target_sum &&
                 nums2[j_lower_bound] <= nums1[std::min(nums1.size() - 1,
                 i_candidate + 1)]) {
        i_upper_bound = i_candidate;
        continue;
      }
    }
    if (j_lower_bound + 2 <= j_upper_bound) {
      size_t j_candidate = (j_lower_bound + j_upper_bound) / 2.0;
      if (j_candidate + i_lower_bound <= target_sum &&
          nums2[j_candidate] <= nums1[std::min(nums1.size() - 1, i_upper_bound +
          1)]) {
        j_lower_bound = j_candidate;
        continue;
      }
      j_candidate = (j_lower_bound + j_upper_bound + 1) / 2.0;
      if (i_upper_bound + j_candidate >= target_sum &&
                 nums1[i_lower_bound] <= nums2[std::min(nums2.size() - 1, j_candidate + 1)]) {
        j_upper_bound = j_candidate;
        continue;
      }
    }
    if (j_lower_bound + 1 == j_upper_bound) {
      size_t j_candidate = j_upper_bound;
      if (j_candidate + i_lower_bound <= target_sum &&
          nums2[j_candidate] <= nums1[std::min(nums1.size() - 1, i_upper_bound + 1)]) {
        j_lower_bound = j_candidate;
        continue;
      }
      j_candidate = j_lower_bound;
      if (i_upper_bound + j_candidate >= target_sum &&
                 nums1[i_lower_bound] <= nums2[std::min(nums2.size() - 1, j_candidate + 1)]) {
        j_upper_bound = j_candidate;
        continue;
      }
    }
    std::cout << "stuck" << std::endl;
    std::cout << "array1 ";
    for (int i : nums1) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "array2 ";
    for (int i : nums2) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "i_lower_bound " << i_lower_bound << std::endl;
    std::cout << "i_upper_bound " << i_upper_bound << std::endl;
    std::cout << "j_lower_bound " << j_lower_bound << std::endl;
    std::cout << "j_upper_bound " << j_upper_bound << std::endl;

    return 137;
  }

  const size_t i = i_lower_bound;
  const size_t j = j_lower_bound;

  /*
  i is the largest index for which nums1[i] <= median
  j is the largest index for which nums2[j] <= median

  There are exactly `i + j + 2` elements smaller than or equal to the median.

  i + j == target_sum
  */

  if ((nums1.size() + nums2.size()) % 2 == 1) {
    return std::max(nums1[i], nums2[j]);
  } else {
    if (nums1.size() == i + 1) {
      return (std::max(nums1[i], nums2[j]) + nums2[j + 1]) / 2.0;
    } else if (nums2.size() == j + 1) {
      return (std::max(nums1[i], nums2[j]) + nums1[i + 1]) / 2.0;
    } else {
      return (std::max(nums1[i], nums2[j]) +
              std::min(nums1[i + 1], nums2[j + 1])) /
             2.0;
    }
  }
};
