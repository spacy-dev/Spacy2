#ifndef SBO_COW_FOOABLE_HH
#define SBO_COW_FOOABLE_HH

namespace SBOCOW
{
    class Fooable
    {
    public:
        int foo() const;
        void set_value(int value);
    };
}
#endif
