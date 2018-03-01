

#include <iomanip>
#include <iostream>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osg/PositionAttitudeTransform>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osgViewer/ViewerEventHandlers>
#include <osgUtil/Optimizer>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <osg/TexMat>
#include <osg/TexGen>
#include <osg/TexEnv>
#include <osg/TexEnvCombine>
#include <osg/TextureCubeMap>

#include <osgUtil/TangentSpaceGenerator>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgSim/OverlayNode>

#include <osgViewer/Viewer>
#include <iostream>

#include <osgShadow/SoftShadowMap>
#include <osg/TextureRectangle>
#include <osgShadow/ShadowedScene>
#include <osg/ShapeDrawable>
#include <osgShadow/ViewDependentShadowMap>
#include <osg/ClipNode>



#include <osg/AnimationPath>
#include <osg/PolygonMode>
#include <osgDB/ReadFile>

#include <osgDB/WriteFile>
#ifdef USE_EFFECT_COMPOSITOR
    #include <osgFX/EffectCompositor>
#endif
#include <osgShadow/SoftShadowMap>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>

#include "TextureVisitor.h"
#include "main.h"

osg::ref_ptr<osg::Uniform> lightUniform;
//fireplace
osg::Vec3d lightPos( 4.059008, 7.935339 , 1);


/////////////////////////////////////////////////////////////////////////////////////////////////////
osg::Geode *createScreenQuad(float width,
                             float height,
                             float scale,
                             osg::Vec3 corner)
{
    osg::Geometry* geom = osg::createTexturedQuadGeometry(
        corner,
        osg::Vec3(width, 0, 0),
        osg::Vec3(0, height, 0),
        0,
        0,
        scale,
        scale);
    osg::ref_ptr<osg::Geode> quad = new osg::Geode;
    quad->addDrawable(geom);
    int values = osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED;
    quad->getOrCreateStateSet()->setAttribute(
        new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,
                             osg::PolygonMode::FILL),
        values);
    quad->getOrCreateStateSet()->setMode(GL_LIGHTING, values);
    return quad.release();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
osg::TextureRectangle *createFloatTextureRectangle(int textureSize)
{
    osg::ref_ptr<osg::TextureRectangle> tex2D = new osg::TextureRectangle;
    tex2D->setTextureSize(textureSize, textureSize);
    tex2D->setInternalFormat(GL_RGBA16F_ARB);
    tex2D->setSourceFormat(GL_RGBA);
    tex2D->setSourceType(GL_FLOAT);
    return tex2D.release();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
osg::Camera *createHUDCamera(double left,
                             double right,
                             double bottom,
                             double top)
{
    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);
    camera->setRenderOrder(osg::Camera::POST_RENDER);
    camera->setAllowEventFocus(false);
    camera->setProjectionMatrix(osg::Matrix::ortho2D(left, right, bottom, top));
    camera->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    return camera.release();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::LightSource> createLight(const osg::Vec3 &pos)
{
    osg::ref_ptr<osg::LightSource> light = new osg::LightSource;
    light->getLight()->setPosition(osg::Vec4(pos.x(), pos.y(), pos.z(), 1));
    light->getLight()->setAmbient(osg::Vec4(0.2, 0.2, 0.2, 1));
    light->getLight()->setDiffuse(osg::Vec4(0.8, 0.8, 0.8, 1));
    return light;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
Pipeline createPipelineEffectCompositor(
        osg::ref_ptr<osg::Group> scene,
        osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene,
        const osg::Vec3 lightPos)
{
    Pipeline p;
    return p;
}
/*
Pipeline createPipelinePlainOSG(
        osg::ref_ptr<osg::Group> scene,
        osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene,
        const osg::Vec3 lightPos)
{
    //Pipeline p;
    //p.rootnode = new osg::Group;
    //p.textureSize = 1024;
    
	int textureSize= 1024;
    osg::ref_ptr<osg::Group> rootnode = new osg::Group;
    osg::ref_ptr<osg::Texture> pass1Shadows;
    osg::ref_ptr<osg::Texture> pass2Colors;
    osg::ref_ptr<osg::Texture> pass2Normals;
    osg::ref_ptr<osg::Texture> pass2Positions;
    osg::ref_ptr<osg::Texture> pass3Final;


	// Pass 1 (shadow).
    pass1Shadows = createFloatTextureRectangle(textureSize);
    osg::ref_ptr<osg::Camera> pass1 =  createRTTCamera(osg::Camera::COLOR_BUFFER, pass1Shadows);
    pass1->addChild(shadowedScene.get());




    // Pass 2 (positions, normals, colors).
    pass2Positions = createFloatTextureRectangle(textureSize);
    pass2Normals   = createFloatTextureRectangle(textureSize);
    pass2Colors    = createFloatTextureRectangle(textureSize);
    osg::ref_ptr<osg::Camera> pass2 =  createRTTCamera(osg::Camera::COLOR_BUFFER0, pass2Positions);
    pass2->attach(osg::Camera::COLOR_BUFFER1, pass2Normals);
    pass2->attach(osg::Camera::COLOR_BUFFER2, pass2Colors);
    pass2->addChild(scene.get());

    osg::StateSet *ss = setShaderProgram(pass2, "pass2.vert", "pass2.frag");
    ss->setTextureAttributeAndModes(0, createTexture("rockwall.png"));
    ss->setTextureAttributeAndModes(1, createTexture("rockwall_NH.png"));
    ss->addUniform(new osg::Uniform("diffMap", 0));
    ss->addUniform(new osg::Uniform("bumpMap", 1));
    // Pass 3 (final).
    p.pass3Final = createFloatTextureRectangle(p.textureSize);
    osg::ref_ptr<osg::Camera> pass3 =
        createRTTCamera(osg::Camera::COLOR_BUFFER, p.pass3Final, true);
    ss = setShaderProgram(pass3, "pass3.vert", "pass3.frag");
    ss->setTextureAttributeAndModes(0, p.pass2Positions);
    ss->setTextureAttributeAndModes(1, p.pass2Normals);
    ss->setTextureAttributeAndModes(2, p.pass2Colors);
    ss->setTextureAttributeAndModes(3, p.pass1Shadows);
    ss->addUniform(new osg::Uniform("posMap",    0));
    ss->addUniform(new osg::Uniform("normalMap", 1));
    ss->addUniform(new osg::Uniform("colorMap",  2));
    ss->addUniform(new osg::Uniform("shadowMap", 3));
    // Light position.
    ss->addUniform(new osg::Uniform("lightPos", lightPos));
    // rootnode.
    rootnode->addChild(pass1);
    rootnode->addChild(pass2);
    rootnode->addChild(pass3);
    return p;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////
osg::Camera *createRTTCamera(osg::Camera::BufferComponent buffer,
                             osg::Texture *tex,
                             bool isAbsolute)
{
    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    //camera->setClearColor(osg::Vec4());
    camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    camera->setRenderOrder(osg::Camera::PRE_RENDER);
    if (tex)
    {
        tex->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
        tex->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
        camera->setViewport(0, 0, tex->getTextureWidth(), tex->getTextureHeight());
        camera->attach(buffer, tex);
    }
    if (isAbsolute)
    {
        camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        camera->setProjectionMatrix(osg::Matrix::ortho2D(0.0, 1.0, 0.0, 1.0));
        camera->setViewMatrix(osg::Matrix::identity());
        camera->addChild(createScreenQuad(1.0f, 1.0f));
    }
    return camera.release();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ComputeTangentVisitor : public osg::NodeVisitor
	{
	public:
		void apply( osg::Node& node ) { traverse(node); }
	    
		void apply( osg::Geode& node )
		{
			for ( unsigned int i=0; i<node.getNumDrawables(); ++i )
			{
				osg::Geometry* geom = dynamic_cast<osg::Geometry*>( node.getDrawable(i) );
				if ( geom ) generateTangentArray( geom ); //setGeometryVertexArray(geom);
			}
			traverse( node );
		}
		
		void generateTangentArray( osg::Geometry* geom )
		{
			osg::ref_ptr<osgUtil::TangentSpaceGenerator> tsg = new osgUtil::TangentSpaceGenerator;
			tsg->generate( geom );
			geom->setVertexAttribArray( 6, tsg->getTangentArray() );
			geom->setVertexAttribBinding( 6, osg::Geometry::BIND_PER_VERTEX );
			geom->setVertexAttribArray( 7, tsg->getBinormalArray() );
			geom->setVertexAttribBinding( 7, osg::Geometry::BIND_PER_VERTEX );
		}
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Group> createSceneRoom()
{
    // Room.
    osg::ref_ptr<osg::MatrixTransform> room = new osg::MatrixTransform;
    
   // osg::ref_ptr<osg::Node> roomModel = osgDB::readNodeFile("G:\\GLOBAL\\20140803-Haunted-House\\models\\STUBS\\room-0-stub.obj");
	
	osg::ref_ptr<osg::Node> roomModel = osgDB::readNodeFile("room.osgb");
	if(!roomModel)
	{
	std::cout<<"Couldn't find room Model"<<std::endl;
	exit(EXIT_FAILURE);
	}		

	lightUniform = new osg::Uniform("lightPos2", lightPos);


	ComputeTangentVisitor ctv;
	ctv.setTraversalMode( osg::NodeVisitor::TRAVERSE_ALL_CHILDREN );
	//roomModel->accept( ctv );
	
	//TextureVisitor texturevisitor;
	//texturevisitor.lightPos = lightPos;
	//texturevisitor.lightUniform = lightUniform;

	
	
	//roomModel->accept(texturevisitor);



   // room->addChild(roomModel);
    //room->setMatrix(osg::Matrix::translate(0, 0, 1));
   
    osg::ref_ptr<osg::Group> scene = new osg::Group;
    scene->addChild(roomModel);
    
    return scene;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
osg::Texture2D *createTexture(const std::string &fileName)
{
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    texture->setImage(osgDB::readImageFile(fileName));
    texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
    texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
    texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
    texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
    return texture.release();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::Camera> createTextureDisplayQuad(
    const osg::Vec3 &pos,
    osg::StateAttribute *tex,
    float scale,
    float width,
    float height)
{
    osg::ref_ptr<osg::Camera> hc = createHUDCamera();
    hc->addChild(createScreenQuad(width, height, scale, pos));
    hc->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex);
    return hc;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void setAnimationPath(osg::ref_ptr<osg::MatrixTransform> node,
                      const osg::Vec3 &center,
                      float time,
                      float radius)
{
    // Create animation.
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
    path->setLoopMode(osg::AnimationPath::LOOP);
    unsigned int numSamples = 32;
    float delta_yaw = 2.0f * osg::PI / (static_cast<float>(numSamples) - 1.0f);
    float delta_time = time / static_cast<float>(numSamples);
    for (unsigned int i = 0; i < numSamples; ++i)
    {
        float yaw = delta_yaw * static_cast<float>(i);
        osg::Vec3 pos(center.x() + sinf(yaw)*radius,
                      center.y() + cosf(yaw)*radius,
                      center.z());
        osg::Quat rot(-yaw, osg::Z_AXIS);
        path->insert(delta_time * static_cast<float>(i),
                     osg::AnimationPath::ControlPoint(pos, rot));
    }
    // Assign it.
    node->setUpdateCallback(new osg::AnimationPathCallback(path));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
osg::ref_ptr<osg::StateSet> setShaderProgram(osg::ref_ptr<osg::Camera> pass,
                                             std::string vert,
                                             std::string frag)
{
    osg::ref_ptr<osg::Program> program = new osg::Program;
    program->addShader(osgDB::readShaderFile(vert));
    program->addShader(osgDB::readShaderFile(frag));
    osg::ref_ptr<osg::StateSet> ss = pass->getOrCreateStateSet();
    ss->setAttributeAndModes(
        program.get(),
        osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
    return ss;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
osg::Geode * createScreenQuad(const osg::Vec3 & corner, float width,
							  float height, float scale)
{
	osg::Geometry * geom =	osg::createTexturedQuadGeometry(corner, osg::Vec3(width, 0.0f, 0.0f),osg::Vec3(0.0f, height, 0.0f), 0.0f,0.0f, width * scale, height * scale);
	osg::ref_ptr < osg::Geode > quad = new osg::Geode;
	quad->addDrawable(geom);

	int values = osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED;
	quad->getOrCreateStateSet()->setAttribute(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::FILL), values);
	quad->getOrCreateStateSet()->setMode(GL_LIGHTING, values);
	return quad.release();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////


int main ( int argc, char **argv )
{
    // Useful declaration.
    //osg::ref_ptr<osg::StateSet> ss;

	int textureSize= 2048;

    osg::ref_ptr<osg::Group> rootnode = new osg::Group;


    // Scene.
    osg::Vec3 lightPos(0, 0, 13);
    osg::ref_ptr<osg::Group> scene = createSceneRoom();
    osg::ref_ptr<osg::LightSource> light = createLight(lightPos);
    scene->addChild(light.get());


	//view the light source 1
	osg::Sphere *unitSphere1 = new osg::Sphere(osg::Vec3(0,0,0),.07);
	osg::ShapeDrawable* unitSphereDrawable1 = new osg::ShapeDrawable(unitSphere1); 
	osg::Geode* geode1 = new osg::Geode;
	geode1->addDrawable(unitSphereDrawable1);
	osg::ref_ptr<osg::PositionAttitudeTransform> lightpat1 = new osg::PositionAttitudeTransform;
	lightpat1->addChild(geode1);
	lightpat1->setPosition(lightPos);

	rootnode->addChild(lightpat1);


    // Shadowed scene.
    osg::ref_ptr<osgShadow::SoftShadowMap> shadowMap = new osgShadow::SoftShadowMap;
    shadowMap->setJitteringScale(8);
    shadowMap->addShader(osgDB::readShaderFile("pass1Shadow.frag"));
    shadowMap->setLight(light);
    osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;
    shadowedScene->setShadowTechnique(shadowMap.get());
    shadowedScene->addChild(scene.get());
    //rootnode->insertChild(0, shadowedScene.get());
	//Pipeline p = createPipelinePlainOSG(scene, shadowedScene, lightPos);
    
	
    osg::ref_ptr<osg::Texture> pass1Shadows;

    osg::ref_ptr<osg::Texture> pass2Colors;
    osg::ref_ptr<osg::Texture> pass2Normals;
    osg::ref_ptr<osg::Texture> pass2Positions;

    osg::ref_ptr<osg::Texture> pass3Final;


	// Pass 1 (shadow).
    pass1Shadows = createFloatTextureRectangle(textureSize);
    osg::ref_ptr<osg::Camera> pass1 =  createRTTCamera(osg::Camera::COLOR_BUFFER, pass1Shadows);
    pass1->addChild(shadowedScene.get());




    // Pass 2 (positions, normals, colors).
    pass2Positions = createFloatTextureRectangle(textureSize);
    pass2Normals   = createFloatTextureRectangle(textureSize);
    pass2Colors    = createFloatTextureRectangle(textureSize);
    osg::ref_ptr<osg::Camera> pass2 =  createRTTCamera(osg::Camera::COLOR_BUFFER0, pass2Positions);
    pass2->attach(osg::Camera::COLOR_BUFFER1, pass2Normals);
    pass2->attach(osg::Camera::COLOR_BUFFER2, pass2Colors);
    pass2->addChild(scene.get());

    osg::ref_ptr<osg::StateSet> ss = setShaderProgram(pass2, "pass2.vert", "pass2.frag");
   // ss->setTextureAttributeAndModes(0, createTexture("rockwall.png"));
    //ss->setTextureAttributeAndModes(1, createTexture("rockwall_NH.png"));
    //ss->addUniform(new osg::Uniform("texture", 0));
    //ss->addUniform(new osg::Uniform("normalTexture", 1));


    // Pass 3 (final).
    pass3Final = createFloatTextureRectangle(textureSize);
    osg::ref_ptr<osg::Camera> pass3 = createRTTCamera(osg::Camera::COLOR_BUFFER, pass3Final, true);
    osg::ref_ptr<osg::StateSet> ss2 = setShaderProgram(pass3, "pass3.vert", "pass3.frag");

    ss2->setTextureAttributeAndModes(0, pass2Positions);
    ss2->setTextureAttributeAndModes(1, pass2Normals);
    ss2->setTextureAttributeAndModes(2, pass2Colors);
    ss2->setTextureAttributeAndModes(3, pass1Shadows);
    ss2->addUniform(new osg::Uniform("posMap",    0));
    //ss2->addUniform(new osg::Uniform("normalMap", 1));
    ss2->addUniform(new osg::Uniform("colorMap",  2));
    ss2->addUniform(new osg::Uniform("shadowMap", 3));
	ss2->addUniform(new osg::Uniform("normalTexture", 1));
    // Light position.
    ss2->addUniform(new osg::Uniform("lightPos", lightPos));


    // rootnode.
	rootnode->insertChild(0, shadowedScene.get());
    rootnode->addChild(pass1);
    rootnode->addChild(pass2);
    rootnode->addChild(pass3);




	// Quads to display 1 pass textures.
    osg::ref_ptr<osg::Camera> qTexN =  createTextureDisplayQuad(osg::Vec3(0, 0.7, 0),pass2Normals,textureSize);
    osg::ref_ptr<osg::Camera> qTexP =  createTextureDisplayQuad(osg::Vec3(0, 0.35, 0), pass2Positions, textureSize);
    osg::ref_ptr<osg::Camera> qTexC =  createTextureDisplayQuad(osg::Vec3(0, 0, 0), pass2Colors, textureSize);
    // Qaud to display 2 pass shadow texture.
    osg::ref_ptr<osg::Camera> qTexS =  createTextureDisplayQuad(osg::Vec3(0.7, 0.7, 0), pass1Shadows, textureSize);
    // Quad to display 3 pass final (screen) texture.
    osg::ref_ptr<osg::Camera> qTexFinal = createTextureDisplayQuad(osg::Vec3(0, 0, 0), pass3Final, textureSize, 1, 1);



	

    // Must be processed before the first pass takes
    // the result into pass1Shadows texture.
    //rootnode->insertChild(0, shadowedScene.get());
    // Quads are displayed in order, so the biggest one (final) must be first,
    // otherwise other quads won't be visible.
    rootnode->addChild(qTexFinal.get());
    rootnode->addChild(qTexN.get());
    rootnode->addChild(qTexP.get());
    rootnode->addChild(qTexC.get());
    rootnode->addChild(qTexS.get());

	


	
	//rootnode->addChild(debugHudCamera0);

	//osgDB::writeNodeFile(*scene,"room.osgb");



    // Display everything.
    osgViewer::Viewer viewer;
   
	viewer.addEventHandler(new osgViewer::StatsHandler);

    viewer.getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
    viewer.setSceneData(rootnode.get());
    //viewer.setRunMaxFrameRate(40);
    viewer.setUpViewInWindow(300, 100, 800, 600);
    return viewer.run();
}

