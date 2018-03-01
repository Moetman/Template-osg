#ifndef TEXTUREVISITOR_CPP
#define TEXTUREVISITOR_CPP

#include "TextureVisitor.h"


static int ReceivesShadowTraversalMask = 0x1;
static int CastsShadowTraversalMask = 0x2;

// Default constructor - initialize searchForName to "" and 
// set the traversal mode to TRAVERSE_ALL_CHILDREN
TextureVisitor::TextureVisitor() : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN) 

{ 
} 

// Constructor that accepts string argument
// Initializes searchForName to user string
// set the traversal mode to TRAVERSE_ALL_CHILDREN
TextureVisitor::TextureVisitor(const std::string &searchName) : 
                                   osg::NodeVisitor(TRAVERSE_ALL_CHILDREN) 

{ 
} 


// Just attaches a Normal texture at this stage

void TextureVisitor::apply(osg::Geode& geode)
{
	std::cout<<"---------------------------------------------"<<std::endl;
	 std::cout<<"\nfound osg::Geode name: "<<geode.getName()<<std::endl<<std::endl;
	 std::cout<<"Num Drawables: "<<geode.getNumDrawables()<<std::endl;
	 
	 //Set the Recieves NodeMask
	 //geode.setNodeMask(ReceivesShadowTraversalMask);
	 //geode.setNodeMask(CastsShadowTraversalMask);
	 for( unsigned int j = 0 ; j < geode.getNumDrawables(); ++j ) {
		 
	 osg::Geometry* geom = dynamic_cast<osg::Geometry* >(geode.getDrawable(j));
	 
	//pass in tex coords in unit 0 to unit 1&2;
	 osg::Vec2Array* newtexcoords;
	 newtexcoords = dynamic_cast<osg::Vec2Array*>(geom->getTexCoordArray (0));
		 if(newtexcoords){
		 geom->setTexCoordArray(1,newtexcoords);	    
		 geom->setTexCoordArray(2,newtexcoords);	
		 }
			if (geom != NULL) 
			{
			 osg::ref_ptr<osg::StateSet> stateset = geom->getOrCreateStateSet();
			 osg::ref_ptr<osg::Texture2D> texture = (osg::Texture2D*) stateset->getTextureAttribute(0,osg::StateAttribute::TEXTURE); 
			
				if(texture)
				 {
					osg::ref_ptr<osg::Image> image = texture->getImage();
					std::cout<<"Image: "<<image->getName()<<std::endl;
					std::cout<<"ImageFileName: "<<image->getFileName ()<<std::endl<<std::endl;
					//Search string for image name and add bump shaders to geode

					std::string myText(image->getFileName ());
					//std::istringstream iss(myText);
					//std::string token;


					//std::string::find_last_of();
					unsigned found = myText.find_last_of("/\\");
					std::string path = myText.substr(0,found);
					
					std::cout << " path: " << myText.substr(0,found) << '\n';
					std::cout << " file: " << myText.substr(found+1) << '\n';
					std::string noext =  myText.substr(found+1);
					unsigned foundext = noext.find_last_of(".");
					std::cout << " file no ext: " << noext.substr(0,foundext) << '\n';
					//Create the path to the Normal texture
					
					std::string filename(noext.substr(0,foundext));
					std::string compareString("WallpaperForties0084_L");
					std::cout << "compare: " <<filename<<" -  "<< compareString  <<std::endl;
					if(filename.compare(compareString) == 0) 
					{
						std::cout << "Alter texture coords" << std::endl;
						float scale = 3.0f;
						osg::Vec2Array* texcoords;// = new osg::Vec2Array;
						osg::Vec2Array* texcoordsNew = new osg::Vec2Array;
						texcoords = dynamic_cast<osg::Vec2Array*>(geom->getTexCoordArray (0));
						if(texcoords)
						  {
						  unsigned int iy;
 
							for(iy=0;iy <texcoords->size();++iy)// having < produces anomalies
							{
       
		
							  osg::Vec2 temp = texcoords->at(iy);
		
							  //texcoordsFar->push_back( osg::Vec2((float)ix/WIDTH,(float)iy/WIDTH));
							  texcoordsNew->push_back( osg::Vec2(temp.x()*scale, temp.y()*scale));
							 
        
							}

 	
							}

						geom->setTexCoordArray(1,texcoordsNew);
					}
					
					std::stringstream filenoext;
					filenoext << path << "\\" << noext.substr(0,foundext) << "_Normal.jpg";
					std::cout << "Load Normal image: " << filenoext.str() << std::endl<< std::endl;
					//Load Image
					osg::ref_ptr < osg::Image > normalimage = osgDB::readImageFile(filenoext.str());
					//normalimage->setName("normal.jpg");
					if (!normalimage)
					{
						std::cout << "Couldn't find normal texture\n" << std::endl;
						exit(EXIT_FAILURE);
					}

					//osg::ref_ptr<osg::Texture2D> normaltexture = new osg::Texture2D;
					//normaltexture->setDataVariance(osg::Object::DYNAMIC);
					//normaltexture->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR);
					//normaltexture->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);
					//normaltexture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
					//normaltexture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
					int textureSize=1024;
					osg::ref_ptr<osg::TextureRectangle> normaltexture = new osg::TextureRectangle;
					normaltexture->setTextureSize(textureSize, textureSize);
					normaltexture->setInternalFormat(GL_RGBA16F_ARB);
					normaltexture->setSourceFormat(GL_RGBA);
					normaltexture->setSourceType(GL_FLOAT);


					//normaltexture->setResizeNonPowerOfTwoHint(false);

					//set coords for unit 3;
					osg::Vec2Array* texcoords = dynamic_cast<osg::Vec2Array*>(geom->getTexCoordArray (0));
					geom->setTexCoordArray(3,texcoords);

					normaltexture->setImage(normalimage);
					stateset->setTextureAttributeAndModes(1, normaltexture,osg::StateAttribute::ON);
					stateset->addUniform(new osg::Uniform("normalTexture", 1));
					
					//stateset->addUniform(lightUniform);

					/*
					if(parallaxmap)
					{
						//Load Image for parallax mapping
						osg::ref_ptr<osg::Image> image = osgDB::readImageFile("G:\\GLOBAL\\20140803-Haunted-House\\models\\COMPLETED\\terrain\\terrain_Height.jpg"); 
						if(!image)
						{
							std::cout<<"Cant find path map"<<std::endl;
							exit(EXIT_FAILURE);
						}
						osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
						texture->setWrap( osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT );
						texture->setWrap( osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT );
						texture->setFilter( osg::Texture::MIN_FILTER, osg::Texture::LINEAR );
						texture->setFilter( osg::Texture::MAG_FILTER, osg::Texture::LINEAR );

						texture->setImage( image );
						osg::ref_ptr<osg::StateSet> bstateset = model->getOrCreateStateSet();
						bstateset->setTextureAttributeAndModes(4, texture.get() );
						bstateset->addUniform(new osg::Uniform("heightTexture", 4));

						osg::ref_ptr<osg::Uniform> eyePositionUniform = new osg::Uniform("eyeVec", osg::Vec3(0.0,1.0,1.0) );
					
					
					
					}
					*/


					//std::cout << " file no ext: " << noext.substr(foundext+1) << '\n';
					/*while(getline(iss, token, '\\') )
					{
						  std::cout << "token: " << token << std::endl;
					}
					*/
				 }
				 else
				 {
				 std::cout<<"No texture"<<std::endl;
				 }
			    
			}       

			
	 }

	 


	 /*
	 std::cout<<"found osg::Geode"<<std::endl;

	 osg::ref_ptr<osg::StateSet> stateset = geode.getOrCreateStateSet();
	 //osg::StateAttribute* stateAttrib =  stateset->getTextureAttribute (0, StateAttribute::Type type);


	 osg::ref_ptr<osg::Texture2D> texture = (osg::Texture2D*) stateset->getTextureAttribute(0,osg::StateAttribute::TEXTURE); 
	 if(texture)
	 {
		osg::ref_ptr<osg::Image> image = texture->getImage();
		std::cout<<"Image: "<<image->getName()<<std::endl;
	 }
	 else
	 {
	 std::cout<<"No texture"<<std::endl;
	 }

	   traverse( geode );

	 */





	   traverse( geode );
}

void TextureVisitor::apply(osg::Group& group)
{

	 std::cout<<"found osg::Group"<<std::endl;

	   traverse( group );
}








#endif

