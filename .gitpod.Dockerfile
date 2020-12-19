FROM gitpod/workspace-full
                    
USER gitpod

RUN pip3 install -U platformio && brew install uncrustify
