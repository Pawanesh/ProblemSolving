#include <iostream>
#include <vector>

double findMedianSortedArrays(const std::vector<int>& nums1, const std::vector<int>& nums2) {

        auto n = nums1.size();
        auto m = nums2.size();
        if (n == 0 && m > 0) {
            if (m%2 == 0) {
                return ( double(nums2[(m/2)-1]) + nums2[m/2] ) / 2;
                }
            else {
                return nums2[m/2];
            }
        }
        
        else if (m == 0 && n > 0) {
             if (n%2 == 0) {
                return ( double(nums1[n/2-1]) + nums1[n/2] ) / 2;
                }
            else {
                return nums1[n/2];
            }
        }
        else if (m==0 && n == 0) {
            return 0.0;
            
        }
        
        auto total = n+m;
        std::vector<int> merge(total, 0);
        std::size_t i = 0;
        std::size_t ni = 0;
        std::size_t mi = 0;

        while (i < total){
            auto ai = nums1[ni];
            auto bi = nums2[mi];
            if (ai <= bi){
                merge[i] = ai;
                ++ni;
            }
            else {
                merge[i] = bi;
                ++mi;
            }
            ++i;
            
            if (ni == n)
                break;
            else if (mi == m) {
                break;
            }
        }
        
       if (mi < m) {
            while(i < total) {
                merge[i] = nums2[mi];
                ++i;
                ++mi;
            }
        }
        else if (ni < n) {
             while(i < total) {
                merge[i] = nums1[ni];
                ++i;
                ++ni;
            }
        }
        
        if (total%2 == 0) {
            auto a = merge[std::size_t(total)/2 - 1];
            auto b = merge[std::size_t(std::size_t(total)/2 )];
            return (double(a) + double(b)) / 2;
        }
        else {
            return merge[std::size_t(std::size_t(total)/2 )];
        }
    }

int main() {
	std::cout << "Median : " << findMedianSortedArrays(std::vector<int>{3,4}, std::vector<int>{}) << std::endl;
	return 0;
}

