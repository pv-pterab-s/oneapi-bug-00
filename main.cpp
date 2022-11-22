// #include "af-emu.hpp"
#include <CL/sycl.hpp>
#include <stdlib.h>

typedef long long dim_t;
typedef struct {
  dim_t dims[4]; dim_t strides[4]; dim_t offset;
} KParam;
template<typename T>
struct Param {
  sycl::buffer<T> *data; KParam info;
};

Param<float> create_param(size_t dims0, size_t dims1 = 1, size_t dims2 = 1, size_t dims3 = 1) {
    Param<float> out;
    out.info.dims[0] = dims0;
    out.info.dims[1] = dims1;
    out.info.dims[2] = dims2;
    out.info.dims[3] = dims3;
    out.info.strides[0] = 1;
    out.info.strides[1] = out.info.strides[0] * out.info.dims[0];
    out.info.strides[2] = out.info.strides[1] * out.info.dims[1];
    out.info.strides[3] = out.info.strides[2] * out.info.dims[2];
    out.info.offset = 0;
    out.data = new sycl::buffer<float, 1>{sycl::range{dims0 * dims1 * dims2 * dims3}};
    return out;
}

template<typename T>
class reorderCreateKernel {
public:
  reorderCreateKernel(sycl::accessor<T, 1> out, sycl::accessor<T, 1> in,
                      const KParam op, const KParam ip,
                      const int d0, const int d1, const int d2, const int d3,
                      sycl::accessor<float, 2> debugAcc)
    : out_(out), in_(in), op_(op), ip_(ip), d0_(d0), d1_(d1), d2_(d2),
      d3_(d3), debugAcc_(debugAcc) {}
  void operator()(sycl::nd_item<2> it) const {
      auto g = it.get_group();

#ifdef FIX1
      debugAcc_[it.get_global_id(0)][it.get_global_id(1)] = 0;
#endif

      const int ow = g.get_group_id(1);
      const int xx = it.get_local_id(0) + g.get_group_id(0) * g.get_local_range(0);
      const int yy = it.get_local_id(1);
      if (xx >= op_.dims[0] || yy >= op_.dims[1]) return;

      int ids[4];
#ifdef FIX2
      ids[1] = xx; ids[0] = yy; ids[2] = 0; ids[3] = ow;
#else
      ids[d0_] = xx; ids[d1_] = yy; ids[d2_] = 0; ids[d3_] = ow;
#endif
      const int oIdx = yy * op_.strides[1] + xx;
      const int iIdx = ids[3] * ip_.strides[3] + ids[2] * ip_.strides[2] + ids[1] * ip_.strides[1] + ids[0];
      out_[oIdx] = in_[ip_.offset + iIdx];
  }
private:
  sycl::accessor<T, 1> out_, in_;
  const KParam op_, ip_;
  const int d0_, d1_, d2_, d3_;
  sycl::accessor<float, 2> debugAcc_;
};

int main(int argc, char **argv) {
    auto platformNo = atoi(argv[1]);
    auto platforms = sycl::platform::get_platforms();
    assert(platforms.size() == 5);
    auto platform = platforms[platformNo];

    auto devices = platform.get_devices();
    assert(devices.size() == 1);
    auto deviceToRunOn = devices[0];

    std::cout << "running on: " << deviceToRunOn.get_info<sycl::info::device::name>() << " -- " << platform.get_info<sycl::info::platform::name>() << std::endl;

    // in and out data
    float inHost[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    auto in = create_param(5, 4, 1, 1);
    {
      const sycl::host_accessor<float> in_data(*in.data);
      for (int i = 0; i < 20; i++) in_data[i] = inHost[i];
    }
    auto out = create_param(4, 5, 1, 1);

    // run the bad kernel
    auto _5x5 = sycl::range{5, 5};
    sycl::buffer<float, 2> debugBuffer(_5x5);
    sycl::queue Q(deviceToRunOn);
    Q.submit([&](sycl::handler &h) {
        sycl::accessor outAcc{*out.data, h}, inAcc{*in.data, h};
        sycl::accessor debugAcc{debugBuffer, h};
        h.parallel_for(sycl::nd_range{_5x5, _5x5},
                       reorderCreateKernel<float>(outAcc, inAcc, out.info,
                                                  in.info, 1, 0, 2, 3,
                                                  debugAcc));
    }).wait();

    // check the results
    const sycl::host_accessor<float> out_data(*out.data);
    const float gold[] = {0.f,  5.f,  10.f, 15.f, 1.f,  6.f, 11.f, 16.f, 2.f,  7.f,  12.f, 17.f, 3.f, 8.f, 13.f, 18.f, 4.f,  9.f,  14.f, 19};
    const int nelems = in.info.dims[0] * in.info.dims[1] * in.info.dims[2] * in.info.dims[3];
    for (int i = 0; i < nelems; i++)
      if (out_data[i] != gold[i]) exit(1);
}
