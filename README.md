# h5rd - a lightweight hdf5 wrapper

h5rd provides c++11 (or later) header-only access to some of the functionality of hdf5. It supports
creating/opening groups, writing data sets of native or also array and compound types with the possibility
to append and also generate VLEN data sets.

## Dependencies
- hdf5

## Usage
### Basic
```cpp
using namespace h5rd;
// create (and possibly overwrite) my_file.h5
auto file = File::create("my_file.h5");
// create a group within that file alongside with its intermediates
auto g = f->createGroup("/my/group/in/the/file");
// write strings
g.write("string_dataset", "[insert something witty here]");
// write vectors of native types
{
  std::vector<short> numbers {1., 2., 3., 4., 5., 6.};
  g.write("numbers", numbers);
}
// read back the vector
{
  std::vector<short> numbers;
  g.read("numbers", numbers);
}
```
### Compound types, appending to data sets
```cpp
using namespace h5rd;
// create (and possibly overwrite) my_file.h5
auto file = File::create("my_file.h5");
// create a group within that file alongside with its intermediates
auto g = f->createGroup("/my/group/in/the/file");
// define a compound type
struct Stuff {
  int a;
  float x;
  std::array<short, 3> xyz;
}
NativeCompoundType nct = NativeCompoundTypeBuilder(sizeof(Stuff), f->ref())
  .insert<decltype(std::declval<Stuff>().a)>("a", offsetof(Stuff, a))
  .insert<decltype(std::declval<Stuff>().x)>("x", offsetof(Stuff, x))
  .insertStdArray<decltype(std::declval<Stuff>().xyz)>("xyz", offsetof(Stuff, xyz))
  .build();
STDCompoundType sct (nct);
// create an 1D extensible data (chunk size 3) set with this type,
// add a compression filter
NBITFilter filter;
auto ds = group.createDataSet("stuff", {3}, {UNLIMITED_DIMS}, nct, sct, {&filter});
{
  Stuff s1{3, 5.f, {{1, 1, 1}}};
  Stuff s2{4, 5.f, {{2, 2, 2}}};
  Stuff s3{6, 7.f, {{3, 3, 3}}};
  std::vector<Stuff> stuffs{s1, s2, s3};
  // append twice
  ds->append({stuffs.size()}, stuffs.data());
  ds->append({stuffs.size()}, stuffs.data());
}
```
