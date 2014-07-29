

inline ImageRect ImageSize::getLocalRect() const
{
	return ImageRect( ImagePos(0) ,ImagePos( width , height ));
}