/*!
*  Copyright (c) 2016 by Contributors
* \file ndarray.h
* \brief definition of ndarray
* \author Chuntao Hong, Zhang Chen
*/

#ifndef MXNETCPP_NDARRAY_H
#define MXNETCPP_NDARRAY_H

#include <memory>
#include <vector>
#include "base.h"
#include "shape.h"

namespace mxnet {
namespace cpp {

enum DeviceType {
  kCPU = 1,
  kGPU = 2,
  kCPUPinned = 3
};

/*!
* \brief Context interface
*/
class Context {
public:
  /*!
  * \brief Context constructor
  * \param type type of the device
  * \param id id of the device
  */
  Context(const DeviceType &type, int id)
    : type_(type), id_(id) {}
  /*!
  * \return the type of the device
  */
  DeviceType GetDeviceType() const { return type_; }
  /*!
  * \return the id of the device
  */
  int GetDeviceId() const { return id_; }

private:
  DeviceType type_;
  int id_;
};

/*!
* \brief struct to store NDArrayHandle
*/
struct NDBlob {
public:
  /*!
  * \brief default constructor
  */
  NDBlob() : handle_(nullptr) {}
  /*!
  * \brief construct with a NDArrayHandle
  * \param handle NDArrayHandle to store
  */
  explicit NDBlob(NDArrayHandle handle) : handle_(handle) {}
  /*!
  * \brief destructor, free the NDArrayHandle
  */
  ~NDBlob() { MXNDArrayFree(handle_); }
  /*!
  * \brief the NDArrayHandle
  */
  NDArrayHandle handle_;

private:
  NDBlob(const NDBlob &);
  NDBlob &operator=(const NDBlob &);
};

/*!
* \brief NDArray interface
*/
class NDArray {
public:
  /*!
  * \brief construct with a none handle
  */
  NDArray();
  /*!
  * \brief construct with a NDArrayHandle
  */
  explicit NDArray(const NDArrayHandle &handle);
  /*!
  * \brief construct a new dynamic NDArray
  * \param shape the shape of array
  * \param constext context of NDArray
  * \param delay_alloc whether delay the allocation
  */
  NDArray(const std::vector<mx_uint> &shape, const Context &context,
    bool delay_alloc = true);
  /*!
  * \brief construct a new dynamic NDArray
  * \param shape the shape of array
  * \param constext context of NDArray
  * \param delay_alloc whether delay the allocation
  */
  NDArray(const Shape &shape, const Context &context,
    bool delay_alloc = true);

  NDArray(const mx_float *data, size_t size);
  explicit NDArray(const std::vector<mx_float> &data);
  // TODO(zhangcheng-qinyinghua)
  // implement all the operators
  NDArray operator+(mx_float scalar);
  NDArray operator-(mx_float scalar);
  NDArray operator*(mx_float scalar);
  NDArray operator/(mx_float scalar);
  NDArray operator+(const NDArray &);
  NDArray operator-(const NDArray &);
  NDArray operator*(const NDArray &);
  NDArray operator/(const NDArray &);
  /*!
  * \brief set all the elements in ndarray to be scalar
  * \param scalar the scalar to set
  * \return reference of self
  */
  NDArray &operator=(mx_float scalar);
  /*!
  * \brief elementwise add to current space
  *  this mutate the current NDArray
  * \param scalar the data to add
  * \return reference of self
  */
  NDArray &operator+=(mx_float scalar);
  /*!
  * \brief elementwise subtract from current ndarray
  * this mutate the current NDArray
  * \param scalar the data to substract
  * \return reference of self
  */
  NDArray &operator-=(mx_float scalar);
  /*!
  * \brief elementwise multiplication to current ndarray
  *  this mutate the current NDArray
  * \param scalar the data to substract
  * \return reference of self
  */
  NDArray &operator*=(mx_float scalar);
  /*!
  * \brief elementwise division from current ndarray
  *  this mutate the current NDArray
  * \param scalar the data to substract
  * \return reference of self
  */
  NDArray &operator/=(mx_float scalar);
  /*!
  * \brief elementwise add to current space
  *  this mutate the current NDArray
  * \param src the data to add
  * \return reference of self
  */
  NDArray &operator+=(const NDArray & src);
  /*!
  * \brief elementwise subtract from current ndarray
  * this mutate the current NDArray
  * \param src the data to substract
  * \return reference of self
  */
  NDArray &operator-=(const NDArray & src);
  /*!
  * \brief elementwise multiplication to current ndarray
  *  this mutate the current NDArray
  * \param src the data to substract
  * \return reference of self
  */
  NDArray &operator*=(const NDArray & src);
  /*!
  * \brief elementwise division from current ndarray
  *  this mutate the current NDArray
  * \param src the data to substract
  * \return reference of self
  */
  NDArray &operator/=(const NDArray & src);
  /*!
  * \brief Do a synchronize copy from a continugous CPU memory region.
  *
  *  This function will call WaitToWrite before the copy is performed.
  *  This is useful to copy data from existing memory region that are
  *  not wrapped by NDArray(thus dependency not being tracked).
  *
  * \param data the data source to copy from.
  * \param size the memory size we want to copy from.
  */
  void SyncCopyFromCPU(const mx_float *data, size_t size);
  /*!
  * \brief Do a synchronize copy from a continugous CPU memory region.
  *
  *  This function will call WaitToWrite before the copy is performed.
  *  This is useful to copy data from existing memory region that are
  *  not wrapped by NDArray(thus dependency not being tracked).
  *
  * \param data the data source to copy from, int the form of mx_float vector
  */
  void SyncCopyFromCPU(const std::vector<mx_float> &data);
  /*!
  * \brief Do a synchronize copy to a continugous CPU memory region.
  *
  *  This function will call WaitToRead before the copy is performed.
  *  This is useful to copy data from existing memory region that are
  *  not wrapped by NDArray(thus dependency not being tracked).
  *
  * \param data the data source to copyinto.
  * \param size the memory size we want to copy into.
  */
  void SyncCopyToCPU(mx_float *data, size_t size);
  /*!
  * \brief return a new copy this NDArray
  * \param context the new context of this NDArray
  * \return the new copy
  */
  NDArray Copy(const Context &) const;
  /*!
  * \brief return offset of the element at (h, w)
  * \param h height position
  * \param w width position
  * \return offset of two dimensions array
  */
  size_t Offset(size_t h = 0, size_t w = 0) const;
  /*!
  * \brief return value of the element at (h, w)
  * \param h height position
  * \param w width position
  * \return value of two dimensions array
  */
  mx_float At(size_t h, size_t w) const;
  /*!
  * \brief Slice a NDArray
  * \param begin begin index in first dim
  * \param end end index in first dim
  * \return sliced NDArray
  */
  NDArray Slice(mx_uint begin, mx_uint end) const;
  /*!
  * \brief Block until all the pending write operations with respect
  *    to current NDArray are finished, and read can be performed.
  */
  void WaitToRead() const;
  /*!
  * \brief Block until all the pending read/write operations with respect
  *    to current NDArray are finished, and write can be performed.
  */
  void WaitToWrite();
  /*!
  * \brief Block until all the pending read/write operations with respect
  *    to current NDArray are finished, and read/write can be performed.
  */
  static void WaitAll();
  /*!
  * \brief Sample gaussian distribution for each elements of out.
  * \param mu mean of gaussian distribution.
  * \param sigma standard deviation of gaussian distribution.
  * \param out output NDArray.
  */
  static void SampleGaussian(mx_float mu, mx_float sigma, NDArray *out);
  /*!
  * \brief Sample uniform distribution for each elements of out.
  * \param begin lower bound of distribution.
  * \param end upper bound of distribution.
  * \param out output NDArray.
  */
  static void SampleUniform(mx_float begin, mx_float end, NDArray *out);
  /*!
  * \return the shape of current NDArray, in the form of mx_uint vector
  */
  std::vector<mx_uint> GetShape() const;
  /*!
  * \return the data pointer to the current NDArray
  */
  const mx_float* GetData() const;

  /*!
  * \return the context of NDArray
  */
  Context GetContext() const;

  /*!
  * \return the NDArrayHandle of the current NDArray
  */
  NDArrayHandle GetHandle() const { return blob_ptr_->handle_; }

private:
  std::shared_ptr<NDBlob> blob_ptr_;
};

}
}

#endif // MXNETCPP_NDARRAY_H
