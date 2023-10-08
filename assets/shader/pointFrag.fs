#version 450 core

out vec4 FragColor;

void main()
{
    if(gl_FragCoord.x < 400)
    {
        FragColor = vec4(0,1,1,1);
    }
    else{
        FragColor = vec4(1,0,1,1);
        
    }
    
}