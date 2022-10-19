#version 330 core
in vec2 TexCoords;
in vec2 pos;
out vec4 color;

uniform sampler2D screenTexture;


void main()
{
    //https://stackoverflow.com/questions/71123043/opengl-texture-issue-on-plane-drawn-using-gldrawarrays
    //float xs = 1024;    //fbo width
    //float ys = 768; //fbo height
    //float r = 10.0; //blur radius
    //
    //float x,y,xx,yy,rr=r*r,dx,dy,w,w0;
    //w0=0.3780/pow(r,1.975);
    //vec2 p;
    //vec4 col=vec4(0.0,0.0,0.0,0.0);
    //for (dx=1.0/xs,x=-r,p.x=0.5+(pos.x*0.5)+(x*dx);x<=r;x++,p.x+=dx){ xx=x*x;
    //    for (dy=1.0/ys,y=-r,p.y=0.5+(pos.y*0.5)+(y*dy);y<=r;y++,p.y+=dy){ yy=y*y;
    //        if (xx+yy<=rr)
    //        {
    //            w=w0*exp((-xx-yy)/(2.0*rr));
    //            col+=texture2D(screenTexture,p)*w;
    //        }
    //    }
    //}
    //color=col;

    color = texture(screenTexture, TexCoords);
}

//void main()
//{ 
//    //inverted color = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
//    color = texture(screenTexture, TexCoords);
//
//}