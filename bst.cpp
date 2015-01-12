#include <iostream>
#include <memory>
#include <queue>

namespace dsa
{
    inline namespace traversalMethod
    {
        enum class TraversalMethod
        {
            pre_order, post_order, in_order, breadth_first
        };
    }
    template< typename T >
    struct Node
    {
        std::unique_ptr< Node<T> > left, right;
        Node<T> *parent;
        T value;

        Node( Node<T> const & node ) = delete;
        Node& operator= ( Node const & ) = delete;
        
        constexpr Node(): left{ nullptr }, right{ nullptr }, parent{ nullptr }, value{} {}
        constexpr Node( T const & val ): left{ nullptr }, right { nullptr }, parent{ nullptr }, value { val } {}
        explicit operator T(){ return value; }
    };

    template< typename T, typename Less = std::less<T>>
    struct BinarySearchTree
    {
    private:
        std::unique_ptr< Node<T> > root;
        unsigned int _size;
        Less _less;
    public:
        BinarySearchTree( BinarySearchTree<T, Less> const & bst ) = delete;
        BinarySearchTree& operator=( BinarySearchTree<T, Less> const & ) = delete;

        BinarySearchTree& operator=( BinarySearchTree<T, Less> && bst )
        {
            root.reset( bst.root.release() );
            return *this;
        }
        constexpr BinarySearchTree( Less const & les = Less {} ): root { nullptr }, _size( 0 ), _less{ les } { }
        BinarySearchTree( std::initializer_list<T> lst, Less const &les = Less{} ): BinarySearchTree { les }
        {
            _less = les;
            for( const auto &l : lst ) {
                insert( l );
            }
        }
        
        inline size_t size() const { return _size; }
        void insert( T const & val )
        {
            if( !root ) {
                root.reset( new Node<T> { val } );
            } else {
                insert_node( val );
                ++_size;
            }
        }
        void insert_node( T const & val )
        {
            Node<T> *ptr = root.get();
            while( true ){
                if( _less( val, ptr->value ) ) {
                    if( !ptr->left ) {
                        ( ptr->left ).reset( new Node<T> { val } );
                        ptr->left->parent = ptr;
                        return;
                    } else {
                        ptr = ptr->left.get();
                    }
                } else {
                    if( !ptr->right ) {
                        ( ptr->right ).reset( new Node<T> { val } );
                        ptr->right->parent = ptr;
                        return;
                    } else {
                        ptr = ptr->right.get();
                    }
                }
            }
            ptr = nullptr;
            
        }
        
        bool remove( T const & val )
        {
            if( !root ) return false;
            auto nodeToRemove = find_node( root.get(), val );

            if( !nodeToRemove ) return false;

            auto parentNode = nodeToRemove->parent;
            if( size() == 1 ){ //We have one element in the BST
                root.reset();
                --_size;
                return true;
            } else if( !nodeToRemove->left && !nodeToRemove->right ) { //leaf node
                if( _less( nodeToRemove->value, parentNode->value ) ) {
                    parentNode->left.reset();
                } else {
                    parentNode->right.reset();
                }
            } else if( !nodeToRemove->left && nodeToRemove->right ) { //right subtree, no left subtree
                if( _less( nodeToRemove->value, parentNode->value )) {
                    parentNode->left = std::move( nodeToRemove->right );
                } else {
                    parentNode->right = std::move( nodeToRemove->right );
                }
            } else if( nodeToRemove->left && !nodeToRemove->right ) { //left subtree, no right subtree
                if( _less( nodeToRemove->value, parentNode->value )) {
                    parentNode->right = std::move( nodeToRemove->left );
                } else {
                    parentNode->left.reset( nodeToRemove->left.release() );
                }
            //~ Contains Bug, To Fix!
            } else { // Left and Right subtree available
                auto largestValue = nodeToRemove->left.get();
                while( largestValue->right ){
                    largestValue = largestValue->right.get();
                }
                largestValue->parent->right.reset();
                nodeToRemove->value = largestValue->value;
            }
            --_size;
            return true;
        }

        inline void traverse( TraversalMethod meth = TraversalMethod::pre_order )
        {
            switch( meth ){
                case TraversalMethod::pre_order:
                    pre_order_traversal( root );
                    break;
                case TraversalMethod::post_order:
                    post_order_traversal( root );
                    break;
                case TraversalMethod::in_order:
                    in_order_traversal( root );
                    break;
                case TraversalMethod::breadth_first: default:
                    breadth_first_traversal( root );
            }
        }
        
        void in_order_traversal( std::unique_ptr<Node<T>> & n )
        {
            if( !n ) return;
            in_order_traversal( n->left );
            std::cout << ( T )*n << " ";
            in_order_traversal( n->right );
        }
        
        void breadth_first_traversal( std::unique_ptr<Node<T>> & n )
        {
            Node<T>* ptr = root.get();
            if( !ptr ) return;
            
            std::queue< Node<T>* > q {};
            while( ptr ){
                std::cout << ptr->value << " ";
                if( ptr->left ) {
                    q.push( ptr->left.get() );
                }
                if( ptr->right ) {
                    q.push( ptr->right.get() );
                }
                ptr = nullptr;
                if( !q.empty() ){
                    ptr = q.front();
                    q.pop();
                }
            }
        }
        
        void post_order_traversal( std::unique_ptr<Node<T>> & n )
        {
            if( !n ) return;            
            post_order_traversal( n->left );
            post_order_traversal( n->right );
            std::cout << n->value << " ";
        }
        void pre_order_traversal( std::unique_ptr<Node<T>> & n ){
            if( !n ) return;
            std::cout << n->value << " ";
            pre_order_traversal( n->left );
            pre_order_traversal( n->right );
        }
        inline bool find( T const & val )
        {
            if( !root ) return false;

            return find_node( root.get(), val ) ? true: false;
        }
        
        void to_graphviz( std::ostream & os )
        {
            unsigned int index = 0;
            os << "digraph G\n";
            os << "{\n";
            if( root ) to_graphviz_impl( os, root, index );
            os << "}\n";
        }

        void to_graphviz_impl( std::ostream & os, std::unique_ptr<Node<T>> &ptr, unsigned int &index )
        {
            if( !ptr ) return;

            unsigned int current_index = index;
            
            os << "NODE" << current_index << " [ label = \"" << ptr->value << "\"]\n";

            ++index;
            if( ptr->left ){
                os << "NODE" << current_index << " -> NODE" << index << "\n";
                to_graphviz_impl( os, ptr->left, index );
            }
            if( ptr->right ){
                os << "NODE" << current_index << " -> NODE" << index << "\n";
                to_graphviz_impl( os, ptr->right, index );
            }
        }
        
        Node<T> *find_node( Node<T>* ptr, T const & val )
        {
            if( !ptr ) return nullptr;
            if( ptr->value == val ) return ptr;
            
            if( _less( val, ptr->value )) return find_node( ptr->left.get(), val );
            else return find_node( ptr->right.get(), val );
        }
         
        Node<T>* find_parent( Node<T> *ptr, T const & val )
        {
            if( !ptr ) return nullptr;
            auto parentNode = find_node( ptr, val );
            if( parentNode ) return parentNode->parent;
            return nullptr;
        }
        
        std::tuple<bool, T> find_min() const
        {
            if( !root ) return std::make_tuple( false, T{} );
            return find_min( root.get() );
        }
        
        std::tuple<bool, T> find_min( Node<T> * ptr ) const
        {
            return !ptr->left ? std::make_tuple( true, ptr->value ) : find_min( ptr->left.get() );
        }
        std::tuple<bool, T> find_max() const
        {
            if( !root ) return std::make_tuple( false, T{} );
            return find_max( root.get() );
        }
        
        std::tuple<bool, T> find_max( Node<T> * ptr ) const
        {
            return !ptr->right ? std::make_tuple( true, ptr->value ) : find_max( ptr->right.get() );
        }
    };
}

struct Foo
{
    int _a;
    Foo( int const & a ): _a( a ) {}
    friend bool operator==( Foo const &a, Foo const & f )
    {
        return a._a == f._a;
    }
    friend std::ostream& operator<<( std::ostream & os, Foo const & a )
    {
        return os << a._a;
    }
};

bool compare( Foo const & a, Foo const & f ){
    return a._a < f._a;
}

template< typename T, typename Compare >
using bst = dsa::BinarySearchTree<T, Compare>;

int main()
{
    std::initializer_list< Foo > const init_list { 84, 78, 87, 16, 83, 94, 12, 36, 87, 22, 50, 93, 28, 41, 63, 91, 27, 30, 37, 60, 64, 24, 27, 31, 36, 63, 73, 69, 68, 68 };

    bst<Foo, decltype( compare )*> _bst ( init_list, compare );
    _bst.traverse( dsa::TraversalMethod::breadth_first );
    std::endl( std::cout );
    
    _bst.remove( *init_list.begin() );
    _bst.traverse( dsa::TraversalMethod::in_order );
    std::endl( std::cout );
    
    return 0;
}
